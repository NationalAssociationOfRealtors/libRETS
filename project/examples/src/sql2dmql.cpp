#include <fstream>
#include <iostream>
#include "librets.h"

using namespace librets;
using namespace librets::util;
using std::istream;
using std::ifstream;
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char * argv[])
{
    try
    {
        SqlToDmqlCompiler compiler;
        istream * inputStream;
        istreamPtr fileInputStream;
        if (argc == 1)
        {
            inputStream = &cin;
        }
        else
        {
            fileInputStream.reset(new ifstream(argv[1]));
            inputStream = fileInputStream.get();
        }

        if (compiler.sqlToDmql(*inputStream) == SqlToDmqlCompiler::DMQL_QUERY)
        {
            DmqlQueryPtr query = compiler.GetDmqlQuery();
            cout << "Resource: " << query->GetResource() << endl;
            cout << "Class: " << query->GetClass() << endl;
            cout << "Fields: " << Output(*query->GetFields()) << endl;
            cout << "DMQL: " << query->GetCriterion()->ToDmqlString() << endl;
        }
        else
        {
            cout << "Not a DMQL query" << endl;
        }
    }
    catch (const RetsException & e)
    {
        e.PrintFullReport(cout);
    }
    
    return 0;
}
