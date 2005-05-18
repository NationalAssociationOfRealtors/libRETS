#include "librets.h"
#include <iostream>

using namespace librets;
using std::string;
using std::auto_ptr;
using std::cout;
using std::endl;

int main(int argc, char * argv[])
{
    try
    {
        RetsSessionPtr session(
            new RetsSession("http://demo.crt.realtors.org:6103/rets/login"));
        session->Login("Joe", "Schmoe");

        SearchRequest searchRequest("Property", "RES", "(ListPrice=300000-)");
        SearchResultSetPtr results = session->Search(searchRequest);
        while (results->HasNext())
        {
            cout << "Listing ID: " << results->GetString("ListingID") << endl;
            cout << "Price: " << results->GetString("ListPrice") << endl;
        }
        session->Logout();
    }
    catch (RetsException & e)
    {
        cout << "An error occured: " << e.GetMessage() << endl;
    }
}
