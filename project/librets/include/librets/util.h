#ifndef LIBRETS_UTIL_H
#define LIBRETS_UTIL_H

#include <vector>
#include <locale>
#include "librets/std_forward.h"
#include "librets/RetsObject.h"

namespace librets {
namespace util {

extern const char * WHITESPACE;

bool isEmpty(std::string aString);

int find(const std::string & aString, const std::string & separators,
         int start_index);

std::string join(const StringVector & strings, std::string separator);

bool splitField(const std::string & field, const std::string & delimiter,
                std::string & key, std::string & value);

void readUntilEof(std::istream & inputStream, std::ostream & outputStream);

std::string readIntoString(std::istream & inputStream);

std::string urlEncode(const std::string & aString);

struct RetsObjectEqualTo
{
    bool operator()(const RetsObjectPtr & o1, const RetsObjectPtr & o2) const
    {
        return (*o1 == *o2);
    }
};

template<class T>
bool VectorEquals(const std::vector<T> & v1,
                  const std::vector<T> & v2)
{
    if (v1.size() != v2.size())
    {
        return false;
    }
    
    RetsObjectEqualTo pred;
    bool equal = true;
    typename std::vector<T>::const_iterator i;
    typename std::vector<T>::const_iterator j;
    for (i = v1.begin(), j = v2.begin();
         i != v1.end();
         i++, j++)
    {
        equal &= pred(*i, *j);
    }
    return equal;
}

template<class T, typename _BinaryPredicate>
bool VectorEquals(const std::vector<T> & v1,
                  const std::vector<T> & v2, _BinaryPredicate pred)
{
    if (v1.size() != v2.size())
    {
        return false;
    }
    
    bool equal = true;
    for (int i = 0; i < v1.size(); i++)
    {
        equal &= pred(v1[i], v2[i]);
    }
    return equal;
}

template<typename T>
struct Printer
{
    // The default behaviour is just to call the object's operator<<,
    // so e.g. if we pass T=int, this still works.
    // Of course the point is that we override this default by
    // specialising the Printer class for types which don't have
    // an operator<< (see below)
    static void print(const T& object, std::ostream& out)
    { out << object; }
};


// This class is used to temporarily wrap the object you want to
// output
template<typename T>
class OutputWrapper
{
    const T * pobj;

  public:

    OutputWrapper(const T& object)
        : pobj(&object)
    { }

    friend std::ostream& operator<<
    (std::ostream& out, const OutputWrapper& wrapper)
    { Printer<T>::print(*wrapper.pobj, out); return out; }
};

// A helper function for creating the output wrapper
template<typename T>
OutputWrapper<T> Output(const T& object)
{ return OutputWrapper<T>(object); }


// Specialise for any type of std::vector
// This requires that operator<< is defined for X, i.e. the type of
// object stored in the vector
template<typename X>
struct Printer< std::vector<X> >
{
    static void print(const std::vector<X>& v, std::ostream& out)
    {
        out << "Vector [";
        char * separator = "";
        for(typename std::vector<X>::const_iterator
                i = v.begin(), end = v.end(); i != end; ++i)
        {

            out << separator << *i;
            separator = ", ";
        }
        out << "]";
    }
};

}; };

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
