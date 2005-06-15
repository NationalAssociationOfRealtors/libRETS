header "post_include_hpp"
{
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "librets/RetsAST.h"
#include "librets/sql_forward.h"
#include "librets/DmqlQuery.h"
#include "librets/DmqlExpression.h"
}

header "post_include_cpp"
{
using namespace librets::DmqlExpression;
using boost::algorithm::split;
using boost::algorithm::is_any_of;
}

options
{
    language="Cpp";
    namespace = "librets";
    namespaceStd = "std";
    namespaceAntlr = "antlr";
}

{

void DmqlTreeParser::setResourceAndClass(DmqlQueryPtr query, RefRetsAST ast)
{
    std::string table = ast->getText();
    StringVector components;
    split(components, table, is_any_of(":"));
    if ((components.size() != 3) || (components.at(0) != "data")) {
        throwSemanticException("Invalid table: " + table, ast);
    }

    query->SetResource(components.at(1));
    query->SetClass(components.at(2));
}

void DmqlTreeParser::throwSemanticException(std::string message,
                                            RefRetsAST ast)
{
    throw antlr::SemanticException(message, "", ast->getLine(),
                                   ast->getColumn());
}

}

class DmqlTreeParser extends TreeParser;

options
{
    importVocab = RetsSql;
    defaultErrorHandler = false;
    ASTLabelType = "RefRetsAST";
}

{
    void setResourceAndClass(DmqlQueryPtr query, RefRetsAST ast);
    void throwSemanticException(std::string message, RefRetsAST ast);
    std::string tableName;
}

statement returns [DmqlQueryPtr q]
    { q.reset(new DmqlQuery()); DmqlCriterionPtr c; }
    : #(SELECT columns[q] table_name[q] c=xcriteria)
        { q->SetCriterion(c);}
    ;

columns [DmqlQueryPtr q]
    : #(COLUMNS (column[q])* )
    ;

column [DmqlQueryPtr q]
    : #(COLUMN table:ID col:ID { q->AddField(col->getText()); })
    ;

table_name [DmqlQueryPtr q]
    : id:ID { setResourceAndClass(q, id); tableName = id->getText();}
    ;

xcriteria returns [DmqlCriterionPtr criterion]
    : {tableName == "rets_data_objects"}? criterion=get_object
    | {tableName != "rets_data_objects"}? criterion=criteria
    ;

criteria returns [DmqlCriterionPtr criterion]
    { DmqlCriterionPtr c1; DmqlCriterionPtr c2; }
    : #(OR c1=criteria c2=criteria)     { criterion = logicOr(c1, c2); }
    | #(AND c1=criteria c2=criteria)    { criterion = logicAnd(c1, c2); }
    | #(NOT c1=criteria)                { criterion = logicNot(c1); }
    | #(QUERY_ELEMENT c1=query_element) { criterion = c1; }
    ;

get_object returns [DmqlCriterionPtr criterion]
    : #(QUERY_ELEMENT criterion=query_element)
        { std::cout << "Get object!" << std::endl; }
    ;

query_element returns [DmqlCriterionPtr criterion]
    { std::string n; DmqlCriterionPtr c; }
    : #(GREATER n=field_name c=field_value)
        { criterion = logicAnd(gt(n, c), logicNot(eq(n, c))); }
    | #(LESS n=field_name c=field_value)
        { criterion = logicAnd(lt(n, c), logicNot(eq(n, c))); }
    | #(EQ n=field_name c=field_value)      { criterion = eq(n, c); }
    | #(LTE n=field_name c=field_value)     { criterion = lt(n, c); }
    | #(GTE n=field_name c=field_value)     { criterion = gt(n, c); }
    ;

field_name returns [std::string name]
    : #(COLUMN table:ID col:ID) { name = col->getText(); }
    ;

field_value returns [DmqlCriterionPtr criterion]
    : id:ID     { criterion = literal(id->getText()); }
    | int1:INT  { criterion = literal(int1->getText()); }
    | s:STRING  { criterion = literal(s->getText()); }
    ;
