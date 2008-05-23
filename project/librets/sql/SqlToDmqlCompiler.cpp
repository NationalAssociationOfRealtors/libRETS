/*
 * Copyright (C) 2005-2008 National Association of REALTORS(R)
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

#include <sstream>
#include <boost/algorithm/string.hpp>
#include <antlr/NoViableAltException.hpp>
#include "librets/SqlToDmqlCompiler.h"
#include "librets/RetsSqlException.h"
#include "RetsSqlLexer.hpp"
#include "RetsSqlParser.hpp"
#include "DmqlTreeParser.hpp"
#include "GetObjectTreeParser.hpp"
#include "LookupTreeParser.hpp"
#include "LookupColumnsTreeParser.hpp"

using namespace librets;
using std::string;
using std::istream;
using std::ostringstream;
using std::istringstream;

using antlr::ASTFactory;
using antlr::RecognitionException;
using antlr::RefToken;
using antlr::SemanticException;
using antlr::NoViableAltException;
using antlr::ANTLRException;

namespace ba = boost::algorithm;


SqlToDmqlCompiler::SqlToDmqlCompiler(SqlMetadataPtr metadata)
  : mMetadata(metadata)
{
}

SqlToDmqlCompiler::QueryType SqlToDmqlCompiler::sqlToDmql(string sql)
{
    istringstream inputStream(sql);
    return sqlToDmql(inputStream);
}

SqlToDmqlCompiler::QueryType SqlToDmqlCompiler::sqlToDmql(istream & inputStream)
{
    try
    {
        RetsSqlLexer lexer(inputStream);
        RetsSqlParser parser(lexer);

        ASTFactory astFactory("RetsAST", &RetsAST::factory);
        parser.initializeASTFactory(astFactory);
        parser.setASTFactory(&astFactory);

        parser.sql_statements();
        string tableName = parser.getTableName()->getText();
        // std::cerr << parser.getAST()->toStringList() << std::endl;
        if (ba::starts_with(tableName, "data:"))
        {
            DmqlTreeParser treeParser;
            treeParser.initializeASTFactory(astFactory);
            treeParser.setASTFactory(&astFactory);
            treeParser.setMetadata(mMetadata);
            
            RefRetsAST ast = RefRetsAST(parser.getAST());
            mDmqlQuery = treeParser.statement(ast);
            return DMQL_QUERY;
        }
        else if (ba::starts_with(tableName, "object:"))
        {
            GetObjectTreeParser treeParser;
            treeParser.initializeASTFactory(astFactory);
            treeParser.setASTFactory(&astFactory);
            
            RefRetsAST ast = RefRetsAST(parser.getAST());
            mGetObjectQuery = treeParser.statement(ast);
            return GET_OBJECT_QUERY;
        }
        else if (ba::starts_with(tableName, "lookup:"))
        {
            LookupTreeParser treeParser;
            treeParser.initializeASTFactory(astFactory);
            treeParser.setASTFactory(&astFactory);

            RefRetsAST ast = RefRetsAST(parser.getAST());
            mLookupQuery = treeParser.statement(ast);
            return LOOKUP_QUERY;
        }
        else if (ba::starts_with(tableName, "lookupcolumns:"))
        {
            LookupColumnsTreeParser treeParser;
            treeParser.initializeASTFactory(astFactory);
            treeParser.setASTFactory(&astFactory);

            RefRetsAST ast = RefRetsAST(parser.getAST());
            mLookupColumnsQuery = treeParser.statement(ast);
            return LOOKUP_COLUMNS_QUERY;
        }
        else
        {
            RefToken token = parser.getTableName();
            throw SemanticException("Invalid table: " + tableName, "",
                                    token->getLine(), token->getColumn());
        }
    }
    catch (NoViableAltException & e)
    {
        ostringstream message;
        int line = e.getLine();
        int column = e.getColumn();
        if ((line == -1) && (column == -1) && (e.node))
        {
            RefRetsAST retsNode = RefRetsAST(e.node);
            line = retsNode->getLine();
            column = retsNode->getColumn();
        }
        message << "line " << line << ":" << column
            << ": " << e.getMessage();
        LIBRETS_THROW(RetsSqlException, (message.str()));
    }
    catch(RecognitionException & e)
    {
        ostringstream message;
        message << "line " << e.getLine() << ":" << e.getColumn()
                << ": " << e.getMessage();
        LIBRETS_THROW(RetsSqlException, (message.str()));
    }
    catch(ANTLRException & e)
    {
        throw RetsSqlException(e.getMessage());
    }
}

DmqlQueryPtr SqlToDmqlCompiler::GetDmqlQuery() const
{
    return mDmqlQuery;
}

GetObjectQueryPtr SqlToDmqlCompiler::GetGetObjectQuery() const
{
    return mGetObjectQuery;
}

LookupQueryPtr SqlToDmqlCompiler::GetLookupQuery() const
{
    return mLookupQuery;
}

LookupColumnsQueryPtr SqlToDmqlCompiler::GetLookupColumnsQuery() const
{
    return mLookupColumnsQuery;
}
