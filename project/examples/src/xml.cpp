#include <iostream>
#include <fstream>
#include <sstream>
#include "librets.h"

using namespace librets;
using namespace std;
namespace b = boost;

typedef boost::shared_ptr<ifstream> ifstreamPtr;

int main(int argc, char * argv[])
{
    try
    {
        RetsXmlParserPtr xmlParser;

        if (argc == 2)
        {
            ifstreamPtr infile(new ifstream(argv[1]));
            xmlParser = RetsXmlParser::CreateDefault(infile);
        }
        else
        {
            istreamPtr xml(new istringstream(
                "<one attr=\"value\">foo"
                "  <two attr1=\"value1\" attr2=\"value2\">bar</two>"
                "</one>\n"));
            
            xmlParser = RetsXmlParser::CreateDefault(xml);
        }

        while (xmlParser->HasNext())
        {
            RetsXmlEventPtr event = xmlParser->GetNextEvent();
            cout << event << endl;
        }
    }
    catch (RetsException & e)
    {
        cout << "Exception: " << e.GetMessage() << endl;
    }
}
