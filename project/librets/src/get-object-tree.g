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

void GetObjectTreeParser::setTable(
        GetObjectQueryPtr query, RefRetsAST ast)
{
    std::string table = ast->getText();
    StringVector components;
    split(components, table, is_any_of(":"));
    if ((components.size() != 3) || (components.at(0) != "object")) {
        throwSemanticException("Invalid table: " + table, ast);
    }

    if (components.at(1) == "location") {
        query->SetUseLocation(true);
    }
    else if (components.at(1) == "binary") {
        query->SetUseLocation(false);
    }
    else {
        throwSemanticException("Invalid table: " + table, ast);
    }

    query->SetResource(components.at(2));
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
        query->AddObjectKey(value);
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
    void setTable(GetObjectQueryPtr query, RefRetsAST ast);
    void handleEquals(GetObjectQueryPtr query,
                      RefRetsAST nameAst, RefRetsAST valueAst);
    void throwSemanticException(std::string message, RefRetsAST ast);

  private:
    std::string tableName;
}

statement returns [GetObjectQueryPtr q]
    { q.reset(new GetObjectQuery()); }
    : #(SELECT table_name[q] columns (criteria[q])?)
    ;

columns
    : #(COLUMNS (id:ID)* )
    ;

table_name [GetObjectQueryPtr q]
    : #(TABLE table:ID alias:ID)
        { setTable(q, alias); tableName = alias->getText();}
    ;

criteria [GetObjectQueryPtr q]
    { RefRetsAST n; }
    : #(OR criteria[q] criteria[q])
    | #(AND criteria[q] criteria[q])
    | #(NOT criteria[q])
    | #(QUERY_ELEMENT n=field_name query_element[q, n])
    | #(IN_ n=field_name in_list[q, n])
    ;

query_element [GetObjectQueryPtr q, RefRetsAST n]
    { RefRetsAST v; }
    : #(GREATER v=field_value)
    | #(LESS v=field_value)
    | #(EQ v=field_value)  { handleEquals(q, n, v); }
    | #(LTE v=field_value)
    | #(GTE v=field_value)
    ;

field_name returns [RefRetsAST name]
    : #(COLUMN table:ID col:ID) { name = col; }
    ;

field_value returns [RefRetsAST value]
    : id:ID     { value = id; }
    | int1:INT  { value = int1; }
    | s:STRING  { value = s; }
    | q:QMARK   { value = q; }
    ;

in_list [GetObjectQueryPtr q, RefRetsAST n]
    { RefRetsAST v; }
    : v=field_value { handleEquals(q, n, v); }
        (v=field_value { handleEquals(q, n, v); })*
    ;
