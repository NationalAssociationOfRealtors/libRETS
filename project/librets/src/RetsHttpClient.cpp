#include "librets/RetsHttpClient.h"
#include "librets/CurlHttpClient.h"

using namespace librets;

RetsHttpClientPtr RetsHttpClient::CreateDefault()
{
    RetsHttpClientPtr httpClient(new CurlHttpClient());
    return httpClient;
}

RetsHttpClient::~RetsHttpClient()
{
}
