#include "librets.h"
#include <iostream>

using namespace librets;
using namespace librets::util;
using namespace std;

void dumpResponse(RetsHttpResponsePtr response);

int main(int argc, char * argv[])
{
    try
    {
        string url = "http://www.example.com/";
        if (argc == 2)
        {
            url = argv[1];
        }

        RetsHttpClientPtr client = RetsHttpClient::CreateDefault();
        RetsHttpRequestPtr request(new RetsHttpRequest());
        request->SetUrl(url);
        RetsHttpResponsePtr response = client->DoRequest(request);
        dumpResponse(response);
    }
    catch (RetsException & e)
    {
        e.PrintFullReport(cout);
    }
}

void dumpResponse(RetsHttpResponsePtr response)
{
    cout << "Response code: " << response->GetResponseCode() << endl;
    istreamPtr inputStream = response->GetInputStream();
    readUntilEof(*inputStream, cout);
    cout << endl;
}
