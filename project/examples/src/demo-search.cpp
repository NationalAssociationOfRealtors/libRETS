#include "librets.h"
#include <iostream>

using namespace librets;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char * argv[])
{
    try
    {
        RetsSessionPtr session(
            new RetsSession("http://demo.crt.realtors.org:6103/rets/login"));
        session->Login("Joe", "Schmoe");
        
        SearchRequestPtr searchRequest(
            new SearchRequest("Property", "ResidentialProperty", 
                              "(ListPrice=300000-)"));
        
        SearchResultSetPtr results = session->Search(searchRequest);
        while (results->HasNext())
        {
            cout << "ListingID: " << results->GetString("ListingID") << endl;
            cout << "ListPrice: " << results->GetString("ListPrice") << endl;
            cout << " Bedrooms: " << results->GetString("Beds") << endl;
            cout << "     City: " << results->GetString("City") << endl;
            cout << endl;
        }
        
        session->Logout();
    }
    catch (RetsException & e)
    {
        e.PrintFullReport(cerr);
    }
}
