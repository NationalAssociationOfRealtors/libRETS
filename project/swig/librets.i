%module librets

%{
#include "librets.h"
using namespace librets;
using std::vector;
using std::string;
%}

%include "std_string.i"
%include "std_vector.i"
%include "exception.i"

%typemap(ruby, out) vector<string> {
  VALUE arr = rb_ary_new2($1.size()); 
  vector<string>::iterator i = $1.begin(), iend = $1.end();
  for ( ; i!=iend; i++ )
    rb_ary_push(arr, rb_str_new2(i->c_str()));
  $result = arr;
}

%exception {
    try {
        $action
    } catch(std::exception & e) {
        std::string message = std::string(e.what()) + "\n";
        SWIG_exception(SWIG_ValueError, message.c_str());
    } catch(...) {
        SWIG_exception(SWIG_RuntimeError,"Unknown exception\n");
    }
}

%nodefault RetsException;
class RetsException
{
  public:
    virtual std::string GetMessage();
};

class SearchRequest
{
  public:
    SearchRequest(std::string searchType, std::string searchClass,
                  std::string query);
    
    void SetStandardNames(bool standardNames);
};

class SearchResultSet
{
  public:
    bool HasNext();
    
    int GetCount();
    
    // const vector<string> GetColumns();
    
    std::string GetString(int columnIndex);
    
    std::string GetString(std::string columnName);
};

class LogoutResponse
{
  public:
    std::string GetBillingInfo();
    std::string GetLogoutMessage();
    int GetConnectTime();
};

class RetsSession
{
  public:
    RetsSession(std::string loginUrl);

    bool Login(std::string userName, std::string password);

    std::string GetAction();

    SearchRequest CreateSearchRequest(std::string searchType, 
                                      std::string searchClass,
                                      std::string query);

    SearchResultSet Search(SearchRequest request);
    LogoutResponse Logout();
};


/* Local Variables: */
/* mode: c++ */
/* End: */
