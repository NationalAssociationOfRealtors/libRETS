#include <sstream>
#include <boost/algorithm/string.hpp>
#include "librets/SqlToDmqlCompiler.h"
#include "librets/RetsSqlException.h"
#include "RetsSqlLexer.hpp"
#include "RetsSqlParser.hpp"
#include "DmqlTreeParser.hpp"
#include "GetObjectTreeParser.hpp"

using namespace librets;
using std::string;
using std::istream;
using std::ostringstream;
using std::istringstream;

using antlr::ASTFactory;
using antlr::RecognitionException;
using antlr::RefToken;
using antlr::SemanticException;
using antlr::ANTLRException;

namespace ba = boost::algorithm;

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
        if (ba::starts_with(tableName, "data:"))
        {
            DmqlTreeParser treeParser;
            treeParser.initializeASTFactory(astFactory);
            treeParser.setASTFactory(&astFactory);
            
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
        else
        {
            RefToken token = parser.getTableName();
            throw SemanticException("Invalid table: " + tableName, "",
                                    token->getLine(), token->getColumn());
        }
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
