/*
 * Copyright (C) 2008 National Association of REALTORS(R)
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
#include "librets/std_forward.h"
#include "librets/sql_forward.h"
#include "librets/LookupQuery.h"
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

void LookupTreeParser::setTable(LookupQueryPtr query, RefRetsAST ast)
{
    std::string table = ast->getText();
    StringVector components;
    split(components, table, is_any_of(":"));
    if ((components.size() != 3) || (components.at(0) != "lookup")) {
        throwSemanticException("Invalid table: " + table, ast);
    }

    query->SetResource(components.at(1));
    query->SetLookup(components.at(2));
}

void LookupTreeParser::throwSemanticException(std::string message,
                                              RefRetsAST ast)
{
    throw antlr::SemanticException(message, "", ast->getLine(),
                                   ast->getColumn());
}

}


class LookupTreeParser extends TreeParser;

options
{
    importVocab = RetsSql;
    defaultErrorHandler = false;
    ASTLabelType = "RefRetsAST";
}

{
  public:
    void setTable(LookupQueryPtr query, RefRetsAST ast);
    void throwSemanticException(std::string message, RefRetsAST ast);

  private:
    std::string tableName;
}

statement returns [LookupQueryPtr q]
    { q.reset(new LookupQuery()); }
    : #(SELECT table_name[q] columns)
    ;

columns
    : #(COLUMNS (id:ID)* )
    ;
    
table_name [LookupQueryPtr q]
    : #(TABLE table:ID alias:ID)
        { setTable(q, alias); tableName = alias->getText();}
    ;
