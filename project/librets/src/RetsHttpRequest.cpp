#include <sstream>
#include "librets/RetsHttpRequest.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using namespace std;

RetsHttpRequest::RetsHttpRequest()
    : mQueryParameters()
{
    mMethod = GET;
}

RetsHttpRequest::~RetsHttpRequest()
{
}

void RetsHttpRequest::SetUrl(string url)
{
    mUrl = url;
}

string RetsHttpRequest::GetUrl() const
{
    return mUrl;
}

void RetsHttpRequest::SetMethod(Method method)
{
    mMethod = method;
}

RetsHttpRequest::Method RetsHttpRequest::GetMethod() const
{
    return mMethod;
}

void RetsHttpRequest::SetHeader(string name, string value)
{
}

void RetsHttpRequest::SetQueryParameter(string name, string value)
{
    if (value != "")
    {
        mQueryParameters[name] = urlEncode(value);
    }
    else
    {
        mQueryParameters.erase(name);
    }
}

std::string RetsHttpRequest::GetQueryString() const
{
    // return mQueryString;
    ostringstream queryString;
    string separator = "";
    StringMap::const_iterator i;
    for (i = mQueryParameters.begin(); i != mQueryParameters.end(); i++)
    {
        const string & name = i->first;
        const string & value = i-> second;
        queryString << separator << name << "=" << value;
        separator = "&";
    }
    return queryString.str();
}
