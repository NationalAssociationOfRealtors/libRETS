#include "librets/RetsSession.h"
#include "librets/SearchResultSet.h"

using namespace librets;
using std::string;

RetsSession::RetsSession(string login_url)
{
}

bool RetsSession::Login(string user_name, string passwd)
{
    return false;
}

SearchResultSetPtr RetsSession::Search(const SearchRequest & request)
{
    SearchResultSetPtr resultSet;
    resultSet.reset();
    return resultSet;
}

RetsMetadataPtr RetsSession::GetMetadata() const
{
    RetsMetadataPtr metadata;
    metadata.reset();
    return metadata;
}

LogoutResponsePtr RetsSession::Logout()
{
    LogoutResponsePtr logoutResponse;
    logoutResponse.reset();
    return logoutResponse;
}
