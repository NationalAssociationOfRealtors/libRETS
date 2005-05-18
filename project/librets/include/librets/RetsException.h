#ifndef LIBRETS_RETS_EXCEPTION_H
#define LIBRETS_RETS_EXCEPTION_H

#include <string>
#include <exception>

namespace librets {

/**
 * The base class for all libret exceptions.
 */
class RetsException : public std::exception
{
  public:
    /**
     * Creates a new exception with a message string.
     *
     * @param message Exception message
     */
    RetsException(std::string message);

    /**
     * Returns the error message.
     *
     * @return The error message
     */
    virtual std::string GetMessage() const throw();

    /**
     * Returns the error message as a C-style character string.
     *
     * @return The C-style error message
     */
    virtual const char * what() const throw();
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
