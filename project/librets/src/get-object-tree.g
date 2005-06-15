header "post_include_hpp"
{
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include "librets/RetsAST.h"
#include "librets/sql_forward.h"
#include "librets/GetObjectQuery.h"
}

header "post_include_cpp"
{
using boost::algorithm::split;
using boost::algorithm::is_any_of;
using boost::lexical_cast;
}

options
{
    language="Cpp";
    namespace = "librets";
    namespaceStd = "std";
    namespaceAntlr = "antlr";
}

{

void GetObjectTreeParser::setResource(
        GetObjectQueryPtr query, RefRetsAST ast)
{
    std::string table = ast->getText();
    StringVector components;
    split(components, table, is_any_of(":"));
    if ((components.size() != 2) || (components.at(0) != "object")) {
        throwSemanticException("Invalid table: " + table, ast);
    }

    query->SetResource(components.at(1));
}

void GetObjectTreeParser::handleEquals(
        GetObjectQueryPtr query, RefRetsAST nameAst, RefRetsAST valueAst)
{
    std::string name = nameAst->getText();
    std::string value = valueAst->getText();
    if (name == "type") {
        query->SetType(value);
    }
    else if (name == "object_key") {
        query->SetObjectKey(value);
    }
    else if (name == "object_id") {
        query->AddObjectId(lexical_cast<int>(value));
    }
    else {
        throwSemanticException("Invalid field: " + name, nameAst);
    }

}

void GetObjectTreeParser::throwSemanticException(std::string message,
                                                 RefRetsAST ast)
{
    throw antlr::SemanticException(message, "", ast->getLine(),
                                   ast->getColumn());
}

}

class GetObjectTreeParser extends TreeParser;

options
{
    importVocab = RetsSql;
    defaultErrorHandler = false;
    ASTLabelType = "RefRetsAST";
}

{
  public:
    void setResource(GetObjectQueryPtr query, RefRetsAST ast);
    void handleEquals(GetObjectQueryPtr query,
                      RefRetsAST nameAst, RefRetsAST valueAst);
    void throwSemanticException(std::string message, RefRetsAST ast);

  private:
    std::string tableName;
}

statement returns [GetObjectQueryPtr q]
    { q.reset(new GetObjectQuery()); }
    : #(SELECT columns table_name[q] criteria[q])
    ;

columns
    : #(COLUMNS (id:ID)* )
    ;

table_name [GetObjectQueryPtr q]
    : id:ID { setResource(q, id); tableName = id->getText();}
    ;

criteria [GetObjectQueryPtr q]
    : #(OR criteria[q] criteria[q])
    | #(AND criteria[q] criteria[q])
    | #(NOT criteria[q])
    | #(QUERY_ELEMENT query_element[q])
    ;

query_element [GetObjectQueryPtr q]
    { RefRetsAST n, v; }
    : #(GREATER n=field_name v=field_value)
    | #(LESS n=field_name v=field_value)
    | #(EQ n=field_name v=field_value) { handleEquals(q, n, v); }
    | #(LTE n=field_name v=field_value)
    | #(GTE n=field_name v=field_value)
    ;

field_name returns [RefRetsAST name]
    : #(COLUMN table:ID col:ID) { name = col; }
    ;

field_value returns [RefRetsAST value]
    : id:ID     { value = id; }
    | int1:INT  { value = int1; }
    | s:STRING  { value = s; }
    ;
