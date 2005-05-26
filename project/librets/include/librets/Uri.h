#ifndef LIBRETS_URI_H
#define LIBRETS_URI_H

#include <string>
#include "librets/RetsObject.h"

namespace librets {

class Uri : public RetsObject
{
  public:
    static const int DEFAULT_PORT = 80;

    Uri(std::string uri);

    Uri(std::string uri, std::string baseUri);

    bool IsAbsolute() const;

    std::string GetScheme() const;

    void SetScheme(std::string scheme);

    std::string GetHost() const;

    void SetHost(std::string host);

    int GetPort() const;

    void SetPort(int port);

    std::string GetPath() const;

    void SetPath(std::string path);
    
    static std::string Resolve(std::string uri, std::string baseUri);

    virtual std::ostream & Print(std::ostream & outputStream) const;

    virtual bool Equals(const RetsObject * object) const;

  private:
    void init(const std::string & uri);

    bool mIsAbsolute;
    std::string mScheme;
    std::string mHost;
    int mPort;
    std::string mPath;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
