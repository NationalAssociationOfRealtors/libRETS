#ifndef LIBRETS_CURL_SLIST_H
#define LIBRETS_CURL_SLIST_H

struct curl_slist;

namespace librets {

class CurlSlist
{
  public:

    CurlSlist();

    ~CurlSlist();

    void append(const char * string);

    const curl_slist * slist();

  private:
    curl_slist * mSlist;
};


};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
