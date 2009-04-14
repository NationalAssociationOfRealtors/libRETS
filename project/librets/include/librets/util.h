/*
 * Copyright (C) 2005 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */
#ifndef LIBRETS_UTIL_H
#define LIBRETS_UTIL_H
/**
 * @file util.h
 * (Internal) Contains utilities.
 */
/// @cond MAINTAINER
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

std::string join(std::string left, std::string right, std::string separator);

bool splitField(const std::string & field, const std::string & delimiter,
                std::string & key, std::string & value);

void readUntilEof(istreamPtr inputStream, std::ostream & outputStream);

void readIntoString(istreamPtr inputStream, std::string & aString);

std::string readIntoString(istreamPtr inputStream);

std::string urlEncode(const std::string & aString);

struct RetsObjectEqualTo
{
    bool operator()(const RetsObjectPtr & o1, const RetsObjectPtr & o2) const
    {
        return (*o1 == *o2);
    }

    bool operator()(const RetsObject * o1, const RetsObject * o2) const
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
        const char * separator = "";
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
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
