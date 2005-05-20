#include "librets.h"
#include <iostream>

using namespace librets;
using namespace librets::util;
using namespace std;
namespace b = boost;

void dumpResponse(RetsHttpResponsePtr response);

int main(int argc, char * argv[])
{
    try
    {
        RetsHttpClientPtr client = RetsHttpClient::CreateDefault();
        client->AddDefaultHeader("RETS-Version", "RETS/1.5");
        client->SetUserCredentials("Joe", "Schmoe");

        RetsHttpRequestPtr request(new RetsHttpRequest());
        request->SetUrl("http://demo.crt.realtors.org:6103/rets/login");
        RetsHttpResponsePtr response = client->DoRequest(request);
        dumpResponse(response);

        request.reset(new RetsHttpRequest());
        request->SetMethod(RetsHttpRequest::POST);
        request->SetUrl("http://demo.crt.realtors.org:6103/rets/search");
        request->SetQueryParameter("Format", "COMPACT-DECODED");
        request->SetQueryParameter("StandardNames", "1");
        request->SetQueryParameter("SearchType", "Property");
        request->SetQueryParameter("Class", "RES");
        request->SetQueryParameter("Count", "1");
        request->SetQueryParameter("QueryType", "DMQL2");
        request->SetQueryParameter(
            "Select", "ListingID,ListPrice,City,ListDate");
        request->SetQueryParameter("Query", "(ListPrice=300000-)");
        response = client->DoRequest(request);
        dumpResponse(response);

        request.reset(new RetsHttpRequest());
        request->SetMethod(RetsHttpRequest::GET);
        request->SetUrl("http://demo.crt.realtors.org:6103/rets/logout");
        response = client->DoRequest(request);
        dumpResponse(response);
    }
    catch (RetsException & e)
    {
        cout << "Exception: " << e.GetMessage() << endl;
    }
}

void dumpResponse(RetsHttpResponsePtr response)
{
    cout << "Response code: " << response->GetResponseCode() << endl;
    cout << "RETS-Version header: " << response->GetHeader("RETS-Version")
         << endl;
    istreamPtr inputStream = response->GetInputStream();
    RetsXmlParserPtr xmlParser = RetsXmlParser::CreateDefault(inputStream);

    while (xmlParser->HasNext())
    {
        RetsXmlEventPtr event = xmlParser->GetNextEvent();
        if (event->GetType() == RetsXmlEvent::START_ELEMENT)
        {
            RetsXmlStartElementEventPtr start =
                b::dynamic_pointer_cast<RetsXmlStartElementEvent>(event);
            cout << "Start element: " << start->GetName() << endl;
            RetsXmlAttributeListPtr attributes = start->GetAttributes();
            RetsXmlAttributeList::iterator i;
            for (i = attributes->begin(); i != attributes->end(); i++)
            {
                RetsXmlAttributePtr attribute = *i;
                cout << "Attribute name: " << attribute->GetName()
                     << ", value: " << attribute->GetValue() << endl;
            }
        }
        else if (event->GetType() == RetsXmlEvent::END_ELEMENT)
        {
            RetsXmlEndElementEventPtr end =
                b::dynamic_pointer_cast<RetsXmlEndElementEvent>(event);
            cout << "End element: " << end->GetName() << endl;
        }
        else if (event->GetType() == RetsXmlEvent::TEXT)
        {
            RetsXmlTextEventPtr textEvent =
                b::dynamic_pointer_cast<RetsXmlTextEvent>(event);
            string text = textEvent->GetText();
            if (!isEmpty(text))
            {
                cout << "Text element: <" <<  text << ">\n";
            }
        }
    }

    cout << endl;
}
