/*
 * Copyright (C) 2005 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */
header "post_include_hpp"
{
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "librets/RetsAST.h"
#include "librets/sql_forward.h"
#include "librets/DmqlQuery.h"
#include "librets/DmqlExpression.h"
#include "librets/SqlMetadata.h"
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

void DmqlTreeParser::setMetadata(SqlMetadataPtr metadata)
{
    mMetadata = metadata;
}

void DmqlTreeParser::setTable(DmqlQueryPtr query, RefRetsAST tableAst,
                              RefRetsAST aliasAst)
{
    mTable = tableAst->getText();
    mAlias = aliasAst->getText();
    StringVector components;
    split(components, mTable, is_any_of(":"));
    if ((components.size() != 3) || (components.at(0) != "data")) {
        throwSemanticException("Invalid table: " + mTable, tableAst);
    }

    query->SetResource(components.at(1));
    query->SetClass(components.at(2));
}

void DmqlTreeParser::assertValidTable(RefRetsAST ast)
{
    std::string table = ast->getText();
    if (!table.empty() && (table != mAlias)) {
        throwSemanticException("Invalid table: " + table, ast);
    }
}

bool DmqlTreeParser::fieldIsLookup(std::string field)
{
    return mMetadata->IsLookupColumn(mTable, field);
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
  public:
    void setMetadata(SqlMetadataPtr metadata);

  private:
    void setTable(DmqlQueryPtr query, RefRetsAST tableAst,
                  RefRetsAST aliasAst);
    void assertValidTable(RefRetsAST ast);
    void throwSemanticException(std::string message, RefRetsAST ast);
    bool fieldIsLookup(std::string field);
    std::string mTable;
    std::string mAlias;
    SqlMetadataPtr mMetadata;
}

statement returns [DmqlQueryPtr q]
    { q.reset(new DmqlQuery()); DmqlCriterionPtr c; }
    : #(SELECT table_name[q] columns[q] (c=criteria)?)
        { q->SetCriterion(c);}
    ;

columns [DmqlQueryPtr q]
    : #(COLUMNS (column[q])* )
    ;

column [DmqlQueryPtr q]
    : #(COLUMN
            table:ID    { assertValidTable(table); }
            col:ID      { q->AddField(col->getText()); })
    ;

table_name [DmqlQueryPtr q]
    : #(TABLE table:ID alias:ID)  { setTable(q, table, alias); }
    ;

criteria returns [DmqlCriterionPtr criterion]
    { DmqlCriterionPtr c1; DmqlCriterionPtr c2; std::string n; }
    : #(OR c1=criteria c2=criteria)     { criterion = logicOr(c1, c2); }
    | #(AND c1=criteria c2=criteria)    { criterion = logicAnd(c1, c2); }
    | #(NOT c1=criteria)                { criterion = logicNot(c1); }
    | #(QUERY_ELEMENT n=field_name c1=query_element[n]) { criterion = c1; }
    | #(IN n=field_name c1=or_list[n]) { criterion = c1; }
    ;

query_element [std::string n] returns [DmqlCriterionPtr criterion]
    { DmqlCriterionPtr c; }
    : {fieldIsLookup(n)}?
        c=lookup_element[n]   { criterion = c; }
    | c=standard_element[n]   { criterion = c; }
    ;

lookup_element [std::string n] returns [DmqlCriterionPtr criterion]
    { DmqlCriterionPtr c; }
    : #(GREATER c=field_value)
        { criterion = logicAnd(gt(n, c), logicNot(eq(n, c))); }
    | #(LESS c=field_value)
        { criterion = logicAnd(lt(n, c), logicNot(eq(n, c))); }
    | #(EQ c=field_value)      { criterion = lookupOr(n, c); }
    | #(LTE c=field_value)     { criterion = lt(n, c); }
    | #(GTE c=field_value)     { criterion = gt(n, c); }
    ;

standard_element [std::string n] returns [DmqlCriterionPtr criterion]
    { DmqlCriterionPtr c; }
    : #(GREATER c=field_value)
        { criterion = logicAnd(gt(n, c), logicNot(eq(n, c))); }
    | #(LESS c=field_value)
        { criterion = logicAnd(lt(n, c), logicNot(eq(n, c))); }
    | #(EQ c=field_value)      { criterion = eq(n, c); }
    | #(LTE c=field_value)     { criterion = lt(n, c); }
    | #(GTE c=field_value)     { criterion = gt(n, c); }
    ;

or_list [std::string n] returns [DmqlCriterionPtr criterion]
    { DmqlCriterionPtr c; }
    : {fieldIsLookup(n)}?
        c=lookup_or_list[n]   { criterion = c; }
    | c=standard_or_list[n]   { criterion = c; }
    ;

standard_or_list [std::string n] returns [DmqlCriterionPtr criterion]
    { DmqlCriterionPtr c; }
    : c=field_value { criterion = eq(n, c); }
        (c=field_value {criterion = logicOr(criterion, eq(n, c)); })*
    ;

lookup_or_list [std::string n] returns [DmqlCriterionPtr criterion]
    { DmqlCriterionPtr c; }
    : c=field_value { criterion = lookupOr(n, c); }
        (c=field_value { criterion = logicOr(criterion, lookupOr(n, c)); })*
    ;

field_name returns [std::string name]
    : #(COLUMN
            table:ID    { assertValidTable(table); }
            col:ID)     { name = col->getText(); }
    ;

field_value returns [DmqlCriterionPtr criterion]
    : id:ID     { criterion = literal(id->getText()); }
    | int1:INT  { criterion = literal(int1->getText()); }
    | s:STRING  { criterion = literal(s->getText()); }
    ;
