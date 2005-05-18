#include <sstream>
#include "librets/SqlToDmqlCompiler.h"
#include "librets/RetsSqlException.h"
#include "RetsSqlLexer.hpp"
#include "RetsSqlParser.hpp"
#include "DmqlTreeParser.hpp"

using namespace librets;
using std::string;
using std::istream;
using std::ostringstream;
using std::istringstream;

using antlr::ASTFactory;
using antlr::RecognitionException;
using antlr::ANTLRException;

DmqlQueryPtr SqlToDmqlCompiler::sqlToDmql(string sql)
{
    istringstream inputStream(sql);
    return sqlToDmql(inputStream);
}

DmqlQueryPtr SqlToDmqlCompiler::sqlToDmql(istream & inputStream)
{
    try
    {
        RetsSqlLexer lexer(inputStream);
        RetsSqlParser parser(lexer);

        ASTFactory astFactory("RetsAST", &RetsAST::factory);
        parser.initializeASTFactory(astFactory);
        parser.setASTFactory(&astFactory);

        parser.sql_statements();


        DmqlTreeParser treeParser;
        treeParser.initializeASTFactory(astFactory);
        treeParser.setASTFactory(&astFactory);

        RefRetsAST ast = RefRetsAST(parser.getAST());
        DmqlQueryPtr query = treeParser.statement(ast);
        return query;
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
