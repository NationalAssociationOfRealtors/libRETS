#include "librets/RetsHttpResponse.h"

using namespace librets;
using std::string;

RetsHttpResponse::~RetsHttpResponse()
{
}

string RetsHttpResponse::GetContentType() const
{
    return GetHeader("Content-Type");
}
