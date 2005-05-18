#include "librets/curl.h"
#include "librets/CurlSlist.h"
#include "librets/RetsException.h"

using namespace librets;

#define CLASS CurlSlist

CLASS::CLASS()
{
    mSlist = 0;
}

CLASS::~CLASS()
{
    if (mSlist != 0)
    {
        curl_slist_free_all(mSlist);
    }
}

void CLASS::append(const char * aString)
{
    curl_slist * temp = curl_slist_append(mSlist, aString);
    if (temp == 0)
    {
        throw RetsException("Unable to append to curl_slist");
    }
    mSlist = temp;
}

const curl_slist * CLASS::slist()
{
    return mSlist;
}
