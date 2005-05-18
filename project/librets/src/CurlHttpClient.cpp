#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "librets/RetsHttpRequest.h"
#include "librets/CurlHttpClient.h"
#include "librets/CurlHttpResponse.h"
#include "librets/RetsException.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using namespace std;
namespace ba = boost::algorithm;

#define CURL_ASSERT(_X_) CurlAssert(LIBRETS_ECTXT(), _X_)

CurlHttpClient::CurlHttpClient()
{
    mCurl = curl_easy_init();
    if (!mCurl)
    {
        throw RetsException("Could not allocate Curl handle");
    }

    try
    {
        CurlAssert(
            LIBRETS_ECTXT(),
            curl_easy_setopt(mCurl, CURLOPT_ERRORBUFFER, mCurlErrorBuffer),
            false);
        // CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_VERBOSE, true));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_HTTPAUTH, CURLAUTH_ANY));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_COOKIEFILE, ""));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, this));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION,
                                     CurlHttpClient::WriteData));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_WRITEHEADER, this));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_HEADERFUNCTION,
                                     CurlHttpClient::WriteHeader));
    }
    catch (RetsException &)
    {
        curl_easy_cleanup(mCurl);
        throw;
    }
}

CurlHttpClient::~CurlHttpClient()
{
    curl_easy_cleanup(mCurl);
}

void CurlHttpClient::CurlAssert(const RetsExceptionContext & context,
                                CURLcode errorCode, bool useErrorBuffer)
{
    if (errorCode != CURLE_OK)
    {
        const char * curlError = curl_easy_strerror(errorCode);
        ostringstream message;
        message << "curl error #" << errorCode  << " (" << curlError
                << ")";
        if (useErrorBuffer)
        {
            message << ": " << (char *)(mCurlErrorBuffer);
        }
        RetsException e(message.str());
        e.SetContext(context);
        throw e;
    }
}

void CurlHttpClient::AddDefaultHeader(string name, string value)
{
    string header = name + ": " + value;
    mDefaultHeaderStrings.push_back(header);
    mHeaders.append(header.c_str());
}

void CurlHttpClient::SetUserCredentials(string userName, string password)
{
    mCurlUserpwd = userName + ":" + password;
    CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_USERPWD,
                                 mCurlUserpwd.c_str()));
}

RetsHttpResponsePtr CurlHttpClient::DoRequest(RetsHttpRequestPtr request)
{
    mCurlUrl = request->GetUrl();
    mQueryString = request->GetQueryString();
    if (request->GetMethod() == RetsHttpRequest::GET)
    {
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_HTTPGET, 1));
        if (mQueryString != "")
        {
            mCurlUrl = mCurlUrl + "?" + mQueryString;
        }
    }
    else
    {
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_POSTFIELDS,
                                     mQueryString.c_str()));
    }
    CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_HTTPHEADER, mHeaders.slist()));
    CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_URL, mCurlUrl.c_str()));
    mResponse.reset(new CurlHttpResponse());
    iostreamPtr dataStream(new stringstream());
    mResponse->SetStream(dataStream);
    CURL_ASSERT(curl_easy_perform(mCurl));
    mResponse->SetUrl(request->GetUrl());
    long responseCode;
    CURL_ASSERT(
        curl_easy_getinfo(mCurl, CURLINFO_RESPONSE_CODE, &responseCode));
    mResponse->SetResponseCode(responseCode);
    return mResponse;
}

size_t CurlHttpClient::WriteData(void * buffer, size_t size, size_t nmemb,
                                 void * userData)
{
    CurlHttpClient * client = (CurlHttpClient *) userData;
    CurlHttpResponsePtr response = client->mResponse;
    size_t bytes = size * nmemb;

    response->WriteData((const char *) buffer, bytes);

#if 0
    cout << "size: " << size << ", nmemb: " << nmemb << ", bytes: "
         << bytes << endl;
    string output((const char *) buffer, bytes);
    cout << output;
#endif

    return bytes;
}

size_t CurlHttpClient::WriteHeader(void * buffer, size_t size, size_t nmemb,
                                   void * userData)
{
    CurlHttpClient * client = (CurlHttpClient *) userData;
    CurlHttpResponsePtr response = client->mResponse;
    size_t bytes = size * nmemb;

    string header((const char *) buffer, bytes);
    string name;
    string value;
    if (splitField(header, ":", name, value))
    {
        ba::trim(value);
        response->SetHeader(name, value);
    }

    return bytes;
}
