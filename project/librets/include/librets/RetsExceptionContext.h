#ifndef LIBRETS_RETS_EXCEPTION_CONTEXT_H
#define LIBRETS_RETS_EXCEPTION_CONTEXT_H

#include <string>

namespace librets {
    
class RetsExceptionContext
{
  public:
    RetsExceptionContext();
    RetsExceptionContext(const std::string & fileName, int lineNumber);
    
    bool IsValid() const throw();
    
    std::string GetFileName() const throw();
    
    int GetLineNumber() const throw();
    
  private:
        std::string mFileName;
    int mLineNumber;
};

#define LIBRETS_ECTXT() RetsExceptionContext(__FILE__, __LINE__)

}

#endif
