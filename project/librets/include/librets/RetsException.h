#ifndef LIBRETS_RETS_EXCEPTION_H
#define LIBRETS_RETS_EXCEPTION_H

#include <string>
#include <exception>

#include "librets/RetsExceptionContext.h"

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

    virtual ~RetsException() throw();
    
    virtual std::string GetName() const throw();

    void SetContext(const RetsExceptionContext & context);
    
    RetsExceptionContext GetContext() const throw();

    /**
     * Returns the error message.
     *
     * @return The error message
     */
    virtual std::string GetMessage() const throw();
    
    virtual void PrintContextMessage(std::ostream & outputStream)
        const throw();

    std::string GetContextMessage() const throw();

    virtual void PrintFullReport(std::ostream & outputStream) const throw();
    
    std::string GetFullReport() const throw();

    /**
     * Returns the error message as a C-style character string.
     *
     * @return The C-style error message
     */
    virtual const char * what() const throw();

  private:
    std::string mMessage;
    RetsExceptionContext mContext;
};

#define LIBRETS_THROW(_ExceptionClass_, _ExceptionConstructor_) \
    { _ExceptionClass_ e _ExceptionConstructor_; \
        e.SetContext(LIBRETS_ECTXT()); \
        throw e; }

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
