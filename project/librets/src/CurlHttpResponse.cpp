#include <iostream>
#include "librets/CurlHttpResponse.h"

using namespace librets;
using namespace std;

CurlHttpResponse::CurlHttpResponse()
{
}   

CurlHttpResponse::~CurlHttpResponse()
{
}

void CurlHttpResponse::SetStream(iostreamPtr stream)
{
    mStream = stream;
}

void CurlHttpResponse::WriteData(const char * data, size_t bytes)
{
    mStream->write(data, bytes);
}

void CurlHttpResponse::SetUrl(string url)
{
}

int CurlHttpResponse::GetResponseCode() const
{
    return mResponseCode;
}

void CurlHttpResponse::SetResponseCode(int responseCode)
{
    mResponseCode = responseCode;
}

string CurlHttpResponse::GetHeader(string name) const
{
    StringMap::const_iterator i = mHeaders.find(name);
    if (i != mHeaders.end())
    {
        return i->second;
    }
    else
    {
        return "";
    }
}

void CurlHttpResponse::SetHeader(string name, string value)
{
    mHeaders[name] = value;
}

istreamPtr CurlHttpResponse::GetInputStream() const
{
    return mStream;
}
