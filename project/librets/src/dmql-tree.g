header "post_include_hpp"
{
#include <iostream>
#include "librets/RetsAST.h"
#include "librets/sql_forward.h"
#include "librets/DmqlQuery.h"
#include "librets/DmqlExpression.h"
#include "librets/util.h"
}

header "post_include_cpp"
{
using namespace librets::util;
using namespace librets::DmqlExpression;
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
    StringVectorPtr components = split(ast->getText(), "_");
    if ((components->size() != 3) || (components->at(0) != "rets")) {
        throwSemanticException("Invalid table: " + ast->getText(), ast);
    }

    query->SetResource(components->at(1));
    query->SetClass(components->at(2));
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
    : #(COLUMNS (id:ID {q->AddField(id->getText());})* )
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
    : #(GREATER n=field_name c=field_value) { criterion = gt(n, c); }
    | #(LESS n=field_name c=field_value)    { criterion = lt(n, c); }
    | #(EQ n=field_name c=field_value)      { criterion = eq(n, c); }
    | #(LTE n=field_name c=field_value)
        { criterion = logicOr(lt(n, c), eq(n, c)); }
    | #(GTE n=field_name c=field_value)
        { criterion = logicOr(gt(n, c), eq(n, c)); }
    ;

field_name returns [std::string name]
    : id:ID { name = id->getText(); }
    ;

field_value returns [DmqlCriterionPtr criterion]
    : id:ID     { criterion = literal(id->getText()); }
    | int1:INT  { criterion = literal(int1->getText()); }
    | s:STRING  { criterion = literal(s->getText()); }
    ;
