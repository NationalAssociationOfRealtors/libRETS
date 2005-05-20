#include "librets.h"
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace librets;
using namespace librets::util;
using namespace std;
using boost::lexical_cast;

int main(int argc, char * argv[])
{
    try
    {
        RetsSessionPtr session(
            new RetsSession("http://demo.crt.realtors.org:6103/rets/login"));
        session->Login("Joe", "Schmoe");
    
        GetObjectRequestPtr getObjectRequest(
            new GetObjectRequest("Property", "Photo"));
    
        if (argc == 3)
        {
            getObjectRequest->AddObject(argv[1], lexical_cast<int>(argv[2]));
        }
    
        GetObjectResponsePtr getObjectResponse =
            session->GetObject(getObjectRequest);
    
        ObjectDescriptorPtr objectDescriptor;
        while (objectDescriptor = getObjectResponse->NextObject())
        {
            cerr << objectDescriptor->GetObjectKey() << endl;
            cerr << objectDescriptor->GetObjectId() << endl;
            cerr << objectDescriptor->GetContentType() << endl;
            istreamPtr inputStream = objectDescriptor->GetData();
            readUntilEof(*inputStream, cout);
        }
    
        session->Logout();
    }
    catch (RetsException & e)
    {
        e.PrintFullReport(cerr);
    }
}
