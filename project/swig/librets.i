#if defined(SWIGCSHARP) || \
    defined(SWIGJAVA)   || \
    defined(SWIGPYTHON) || \
    defined (SWIGRUBY)
%module(directors="1") librets
#else
%module librets
#endif

#ifdef SWIGCSHARP
%{
#include "librets_sharp.h"
#include "librets_bridge.h"
%}
#endif

#ifdef SWIGJAVA
%{
#include "librets_bridge.h"
#include "librets/ExceptionErrorHandler.h"
%}
%include "java/boost_shared_ptr.i"
%include "java/typemaps.i"
#endif

#ifdef SWIGRUBY
%{
#include "librets_ruby.h"
#ifdef WIN32
#ifdef close
# undef close
#endif
#endif
%}
#endif

#ifdef SWIGPHP
#define CLASS KLASS
%{
#define KLASS CLASS
#ifdef WIN32
#include "librets/winundef.h"
#endif
#undef MAP_TYPE
%}
%copyctor;
// Yes, this is says php4 in the swig pragma, but it applies to php5
// as well.  Don't ask me, check the swig documentation.
%pragma(php4) phpinfo="
  php_info_print_table_start();
  php_info_print_table_row(2, \"version\", LIBRETS_VERSION);
  php_info_print_table_end();
"
#endif

#ifdef SWIGPERL
%{
#ifdef WIN32
#include <cstdio>
#endif
#undef Copy
#undef GetClassName
#undef GetMessage
#undef GetObject
%}
#endif

%{
#include "librets.h"

using namespace librets;
using namespace librets::util;

#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <string>

using std::vector;
using std::string;
%}

#ifdef SWIGPYTHON
%{
#include <boost/format.hpp>
using boost::format;
%}
#endif

#ifndef SWIGXML
%include "std_string.i"
#ifndef SWIGPHP
%include "std_vector.i"
#else
%include <std_common.i>
%{
#include <vector>
%}

%rename(GetMsg) RetsException::GetMessage() const throw();
%rename(GetMsg) RetsHttpException::GetMessage() const throw();

namespace std {

    template<class T> class vector {
        // add generic typemaps here
      public:
        vector(unsigned int size = 0);
        unsigned int size() const;
        %rename(is_empty) empty;
        bool empty() const;
        void clear();
        %rename(push) push_back;
        void push_back(const T& x);
        %extend {
            T pop() throw (std::out_of_range) {
                if (self->size() == 0)
                    throw std::out_of_range("pop from empty vector");
                T x = self->back();
                self->pop_back();
                return x;
            }
            T get(int i) throw (std::out_of_range) {
                int size = int(self->size());
                if (i>=0 && i<size)
                    return (*self)[i];
                else
                    throw std::out_of_range("vector index out of range");
            }
            void set(int i, const T& x) throw (std::out_of_range) {
                int size = int(self->size());
                if (i>=0 && i<size)
                    (*self)[i] = x;
                else
                    throw std::out_of_range("vector index out of range");
            }
        }
    };
}

#endif
#endif

%include "exception.i"
%include "auto_ptr_release.i"

typedef std::vector<std::string> StringVector;
%template(StringVector) std::vector<std::string>;
%template(ByteVector) std::vector<unsigned char>;

#ifdef SWIGCSHARP
%include "shared_ptr_release.i"
%typemap(ctype)  unsigned char[] "unsigned char*"
%typemap(cstype) unsigned char[] "byte[]"
%typemap(csin)   unsigned char[] "$csinput"
%typemap(imtype) unsigned char[] "byte[]"
%typemap(in)     unsigned char[] {$1 = $input;}
#endif

%typemap(out) std::vector<unsigned char>
{
    
}

#ifdef SWIGJAVA
%typemap(throws, throws="RetsException") RetsException {
    jclass excep = jenv->FindClass("librets/RetsException");
    if (excep)
    {
        jmethodID jmid = jenv->GetMethodID(excep, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
        jthrowable job = (jthrowable) jenv->NewObject(
            excep, jmid,jenv->NewStringUTF(_e.what()),
            jenv->NewStringUTF(_e.GetExtendedMessage().c_str()));
        jenv->Throw(job);
    }
}
%typemap(throws, throws="RetsHttpException") RetsHttpException {
    jclass excep = jenv->FindClass("librets/RetsHttpException");
    if (excep)
    {
        jmethodID jmid = jenv->GetMethodID(excep, "<init>", "(ILjava/lang/String;)V");
        jthrowable job = (jthrowable) jenv->NewObject(
            excep, jmid, _e.GetHttpResult(),
            jenv->NewStringUTF(_e.GetMessage().c_str()));
        jenv->Throw(job);
    }
}
%typemap(throws, throws="RetsReplyException") RetsReplyException {
    jclass excep = jenv->FindClass("librets/RetsReplyException");
    if (excep)
    {
        jmethodID jmid = jenv->GetMethodID(excep, "<init>", "(ILjava/lang/String;Ljava/lang/String;)V");
        jthrowable job = (jthrowable) jenv->NewObject(
            excep, jmid, _e.GetReplyCode(),
            jenv->NewStringUTF(_e.GetMeaning().c_str()),
            jenv->NewStringUTF(_e.GetExtendedMeaning().c_str()));
        jenv->Throw(job);
    }
}
#else
%exception {
    try {
        $action
    } catch(std::exception & e) {
        std::string message = std::string(e.what()) + "\n";
        SWIG_exception(SWIG_ValueError, message.c_str());
    } catch(...) {
        SWIG_exception(SWIG_RuntimeError,"Unknown exception\n");
    }
}

%nodefault RetsException;
class RetsException
{
  public:
    RetsException(std::string message);
    
    RetsException(std::string message, std::string extendedMessage);

    virtual ~RetsException() throw();

    virtual std::string GetName() const throw();

    virtual std::string GetMessage() const throw();

    virtual std::string GetExtendedMessage() const throw();

    std::string GetFullReport() const throw();
};

%nodefault RetsHttpException;
class RetsHttpException : public RetsException
{
  public:
    RetsHttpException(int httpResult, std::string meaning);
    
    virtual ~RetsHttpException() throw();
    
    int GetHttpResult() const throw();
    
    virtual std::string GetMessage() const throw();
    
    virtual std::string GetName() const throw();
};

%nodefault RetsReplyException;
class RetsReplyException : public RetsException
{
  public:
    RetsReplyException(int replyCode, std::string meaning);
    
    RetsReplyException(int replyCode, std::string meaning,
                       std::string extendedMeaning);
    
    virtual ~RetsReplyException() throw();
    
    int GetReplyCode() const throw();
    
    std::string GetMeaning() const throw();
    
    std::string GetExtendedMeaning() const throw();
};

#ifdef SWIGPYTHON
%extend RetsException {
    std::string __repr__() {
        boost::format repr_fmt("<RetsException Message='%1%' ExtendedMessage='%2%'>\n");
        repr_fmt % (*$self).GetMessage();
        repr_fmt % (*$self).GetExtendedMessage();
        return repr_fmt.str();
    }
    std::string __str__() {
        boost::format str_fmt("Error %1%\n%2%\n");
        str_fmt % (*$self).GetMessage();
        str_fmt % (*$self).GetExtendedMessage();
        return str_fmt.str();
    }
}
%extend RetsHttpException {
    std::string __repr__() {
        boost::format repr_fmt("<RetsHttpException HttpResult='%1%' Message='%2%'>\n");
        repr_fmt % std::to_string((*$self).GetHttpResult());
        repr_fmt % (*$self).GetMessage();
        return repr_fmt.str();
    }
    std::string __str__() {
        boost::format str_fmt("HttpResult %1%\n%2%\n");
        str_fmt % std::to_string((*$self).GetHttpResult());
        str_fmt % (*$self).GetMessage();
        return str_fmt.str();
    }
}
%extend RetsReplyException {
    std::string __repr__() {
        boost::format repr_fmt("<RetsReplyException ReplyCode='%1%' Meaning='%2%' ExtendedMeaning='%3%'>\n");
        repr_fmt % std::to_string((*$self).GetReplyCode());
        repr_fmt % (*$self).GetMeaning();
        repr_fmt % (*$self).GetExtendedMeaning();
        return repr_fmt.str();
    }
    std::string __str__() {
        boost::format str_fmt("ReplyCode %1%\n%2%\n%3%\n");
        str_fmt % std::to_string((*$self).GetReplyCode());
        str_fmt % (*$self).GetMeaning();
        str_fmt % (*$self).GetExtendedMeaning();
        return str_fmt.str();
    }
}
#endif

#endif

#ifdef SWIGCSHARP
%insert(runtime) %{
  typedef void (SWIGSTDCALL* CSharpExceptionCallback_tt)(const char *, const char *);
  CSharpExceptionCallback_tt retsExceptionCallback = NULL;

  extern "C" SWIGEXPORT
  void SWIGSTDCALL RetsExceptionRegisterCallback(CSharpExceptionCallback_tt retsCallback) {
    retsExceptionCallback = retsCallback;
  }

  static void SWIG_CSharpSetPendingExceptionRets(const char *msg, const char *extMsg) {
    retsExceptionCallback(msg, extMsg);
  }

  typedef void (SWIGSTDCALL* CSharpExceptionCallback_t)(int, const char *, const char *);
  CSharpExceptionCallback_t retsReplyExceptionCallback = NULL;

  extern "C" SWIGEXPORT
  void SWIGSTDCALL RetsReplyExceptionRegisterCallback(CSharpExceptionCallback_t retsReplyCallback) {
    retsReplyExceptionCallback = retsReplyCallback;
  }

  static void SWIG_CSharpSetPendingExceptionRetsReply(int replyCode, const char *msg, const char *extMsg) {
    retsReplyExceptionCallback(replyCode, msg, extMsg);
  }

  typedef void (SWIGSTDCALL* CSharpHttpExceptionCallback_t)(int, const char *);
  CSharpHttpExceptionCallback_t retsHttpExceptionCallback = NULL;

  extern "C" SWIGEXPORT
  void SWIGSTDCALL RetsHttpExceptionRegisterCallback(CSharpHttpExceptionCallback_t retsCallback) {
    retsHttpExceptionCallback = retsCallback;
  }

  static void SWIG_CSharpSetPendingExceptionHttp(int httpResult, const char *msg) {
    retsHttpExceptionCallback(httpResult, msg);
  }
%}

%pragma(csharp) imclasscode=%{
  class RetsExceptionHelper {
    public delegate void RetsExceptionDelegate(string message, string extendedMessage);
    static RetsExceptionDelegate retsExceptionDelegate =
                                   new RetsExceptionDelegate(SetPendingRetsException);

    [System.Runtime.InteropServices.DllImport("$dllimport", EntryPoint="RetsExceptionRegisterCallback")]
    public static extern
           void RetsExceptionRegisterCallback(RetsExceptionDelegate retsExceptionCallback);

    static void SetPendingRetsException(string message, string extendedMessage) {
      SWIGPendingException.Set(new RetsExceptionNative(message, extendedMessage));
    }

    static RetsExceptionHelper() {
      RetsExceptionRegisterCallback(retsExceptionDelegate);
    }
  }
  static RetsExceptionHelper retsExceptionHelper = new RetsExceptionHelper();
  
  class RetsReplyExceptionHelper {
    public delegate void RetsReplyExceptionDelegate(int replyCode, string message, string extendedMessage);
    static RetsReplyExceptionDelegate retsReplyExceptionDelegate =
                                   new RetsReplyExceptionDelegate(SetPendingRetsException);

    [System.Runtime.InteropServices.DllImport("$dllimport", EntryPoint="RetsReplyExceptionRegisterCallback")]
    public static extern
           void RetsReplyExceptionRegisterCallback(RetsReplyExceptionDelegate retsReplyExceptionCallback);

    static void SetPendingRetsException(int replyCode, string message, string extendedMessage) {
      SWIGPendingException.Set(new RetsReplyExceptionNative(replyCode, message, extendedMessage));
    }

    static RetsReplyExceptionHelper() {
      RetsReplyExceptionRegisterCallback(retsReplyExceptionDelegate);
    }
  }
  static RetsReplyExceptionHelper retsReplyExceptionHelper = new RetsReplyExceptionHelper();

  class RetsHttpExceptionHelper {
    public delegate void RetsHttpExceptionDelegate(int httpResult, string message);
    static RetsHttpExceptionDelegate retsHttpExceptionDelegate =
                                   new RetsHttpExceptionDelegate(SetPendingRetsException);

    [System.Runtime.InteropServices.DllImport("$dllimport", EntryPoint="RetsHttpExceptionRegisterCallback")]
    public static extern
           void RetsHttpExceptionRegisterCallback(RetsHttpExceptionDelegate retsHttpExceptionCallback);

    static void SetPendingRetsException(int httpResult, string message) {
      SWIGPendingException.Set(new RetsHttpExceptionNative(httpResult, message));
    }

    static RetsHttpExceptionHelper() {
      RetsHttpExceptionRegisterCallback(retsHttpExceptionDelegate);
    }
  }
  static RetsHttpExceptionHelper retsHttpExceptionHelper = new RetsHttpExceptionHelper();
%}

%typemap(throws, canthrow=1) RetsException {
  SWIG_CSharpSetPendingExceptionRets($1.GetMessage().c_str(), $1.GetExtendedMessage().c_str());
  return $null;
}

%typemap(throws, canthrow=1) RetsReplyException {
  SWIG_CSharpSetPendingExceptionRetsReply($1.GetReplyCode(), $1.GetMessage().c_str(), $1.GetExtendedMessage().c_str());
  return $null;
}

%typemap(throws, canthrow=1) RetsHttpException {
  SWIG_CSharpSetPendingExceptionHttp($1.GetHttpResult(), $1.GetMessage().c_str());
  return $null;
}
#endif

#ifdef SWIGPHP
%typemap(throws) RetsException {
    #if 0
    // Couldn't make PHP rethrow the appropriate exception, so we have to live with the
    // generic PHP exception for now. The proper way will probably be modeled after 
    // the code below, but we may also need to insert helper classes into the PHP that
    // extend Exception.
    zend_class_entry **pce;

    if (FAILURE == zend_lookup_class("RetsException",strlen("RetsException"), &pce TSRMLS_CC))
    {
        SWIG_PHP_Error(E_ERROR, "Unable to locate class entry for RetsException");
    }
    zend_throw_exception(*pce, (char *)$1.GetMessage().c_str(), 0 TSRMLS_CC);
    #endif

    SWIG_exception(SWIG_ValueError, $1.GetMessage().c_str());
}

%typemap(throws) RetsReplyException {
    SWIG_exception(SWIG_ValueError, $1.GetMessage().c_str());
}

%typemap(throws) RetsHttpException {
    SWIG_exception(SWIG_ValueError, $1.GetMessage().c_str());
}
#endif

class LoginResponse 
{
  public:
    std::string GetMemberName() const;

    std::string GetUserInfo() const;

    std::string GetBroker() const;

    std::string GetMetadataVersion() const;

    std::string GetMetadataTimestamp() const;

    std::string GetMinMetadataTimestamp() const;

    std::string GetOfficeList() const;

    std::string GetBalance() const;

    std::string GetTimeout() const;

    std::string GetPasswordExpire() const;

    std::string GetActionUrl() const;

    std::string GetChangePasswordUrl() const;

    std::string GetGetObjectUrl() const;

    std::string GetLoginUrl() const;

    std::string GetLoginCompleteUrl() const;

    std::string GetLogoutUrl() const;

    std::string GetSearchUrl() const;

    std::string GetGetMetadataUrl() const;

    std::string GetServerInformationUrl() const;

    std::string GetUpdateUrl() const;

    std::string GetPayloadListUrl() const;

    std::string GetUserID() const;

    std::string GetUserClass() const;

    std::string GetUserLevel() const;

    std::string GetAgentCode() const;

    std::string GetBrokerCode() const;

    std::string GetBrokerBranch() const;

    std::string GetMetadataID() const;

    std::string GetWarnPasswordExpirationDays() const;

    std::string GetStandardNamesVersion() const;

    std::string GetVendorName() const;

    std::string GetServerProductName() const;

    std::string GetServerProductVersion() const;

    std::string GetOperatorName() const;

    std::string GetRoleName() const;

    std::string GetSupportContactInformation() const;
};

class CapabilityUrls
{
  public:
    CapabilityUrls(std::string baseUrl);

    virtual ~CapabilityUrls();

    void SetActionUrl(std::string actionUrl);

    std::string GetActionUrl() const;

    void SetChangePasswordUrl(std::string changePasswordUrl);

    std::string GetChangePasswordUrl() const;

    void SetGetObjectUrl(std::string getObjectUrl);

    std::string GetGetObjectUrl() const;

    void SetLoginUrl(std::string loginUrl);

    std::string GetLoginUrl() const;

    void SetLoginCompleteUrl(std::string loginCompleteUrl);

    std::string GetLoginCompleteUrl() const;

    void SetLogoutUrl(std::string logoutUrl);

    std::string GetLogoutUrl() const;

    void SetSearchUrl(std::string searchUrl);

    std::string GetSearchUrl() const;

    void SetGetMetadataUrl(std::string getMetadataUrl);

    std::string GetGetMetadataUrl() const;

    void SetServerInformationUrl(std::string serverInformationUrl);

    std::string GetServerInformationUrl() const;

    void SetUpdateUrl(std::string updateUrl);

    std::string GetUpdateUrl() const;

    void SetPayloadListUrl(std::string payloadListUrl);

    std::string GetPayloadListUrl() const;
};

class SearchRequest
{
  public:
    SearchRequest(std::string searchType, std::string searchClass,
                  std::string query);
    
    void SetStandardNames(bool standardNames);

    void SetSelect(std::string select);
    
    static const int LIMIT_DEFAULT = -1;
    static const int LIMIT_NONE = 0;
    void SetLimit(int limit);
    
    static const int OFFSET_NONE = 0;
    void SetOffset(int offset);
    
    enum CountType
    {
        NO_RECORD_COUNT,
        RECORD_COUNT_AND_RESULTS,
        RECORD_COUNT_ONLY
    };
    void SetCountType(CountType countType);
    
    enum QueryType
    {
        DMQL,
        DMQL2
    };
    void SetQueryType(QueryType queryType);

    enum FormatType
    {
        COMPACT,
        COMPACT_DECODED
    };
    void SetFormatType(FormatType formatType);

    // This is really from RetsHttpRequest, but SearchRequest is a
    // child of that language, so this should work.  Until we need to,
    // we won't expose RetsHttpRequest
    std::string GetQueryString() const;

    void SetRestrictedIndicator(std::string restrictedIndicator);
    void SetPayload(std::string payload);
};
typedef std::auto_ptr<SearchRequest> SearchRequestAPtr;
#ifndef SWIGPHP
%template(SearchRequestAPtr) std::auto_ptr<SearchRequest>;
#endif

#ifdef SWIGCSHARP
    %typemap(cscode) SearchResultSet %{
        public void SetDataAsArray(byte[] bytes)
        {
            SetInputData(new BinaryData(bytes, bytes.Length));
        }
    %}
#endif

#ifdef SWIGJAVA

    // map java parameter to native declaration in libretsJNI.java
    %typemap(jtype)  unsigned char[] "byte[]"

    // map java parameter (generated java wrapper)

    %typemap(jstype) unsigned char[] "byte[]"

    // identified parameter in the generated java wrapper
    %typemap(javain)   unsigned char[] "$javainput"
    %typemap(jni) unsigned char[] "jbyteArray"

    // Do the input conversion. This will allocate a temporary buffer and
    // copy the data from the Java byte array into it.
    %typemap(in)     unsigned char[] (int _length)
    {
        // Determine the length of the data 
        _length = JCALL1(GetArrayLength, jenv, $input);
        // Allocate a buffer to which the data will be copied 
        try
        {
            $1 = new unsigned char [_length];
            JCALL4(GetByteArrayRegion, jenv, $input, 0, _length, (jbyte *)$1);
        }
        catch(std::exception &e)
        {
            std::string message = std::string(e.what()) + "\n";
            SWIG_JavaException(jenv, SWIG_ValueError, message.c_str()); 
            return $null;
        }
        catch(...)
        {
            SWIG_JavaException(jenv, SWIG_RuntimeError, "Unknown exception\n"); 
            return $null;
        }
    }

    %typemap(arginit) unsigned char []
    {
        $1 = NULL;
    }

    // The data has been copied to the temporary buffer. Now write it back to
    // the Java domain and then release the buffer.
    %typemap(argout) unsigned char [] 
    {
        JCALL4(SetByteArrayRegion, jenv, $input, 0, _length$argnum, (const jbyte *)$1);
        if ($1)
        {
            try
            {
                delete $1;
            }
            catch(std::exception &e)
            {
                std::string message = std::string(e.what()) + "\n";
                SWIG_JavaException(jenv, SWIG_ValueError, message.c_str()); 
                return $null;
            }
            catch(...)
            {
                SWIG_JavaException(jenv, SWIG_RuntimeError, "Unknown exception\n"); 
                return $null;
            }
        }
    }

    class BinaryData
    {
      public:
            BinaryData(unsigned char buffer[], int length);
            int Size() const;
            const char * AsChar() const;
            void Copy(unsigned char buffer[], int length) const; 
    };

    typedef std::auto_ptr<BinaryData> BinaryDataAPtr;
    SWIG_AUTO_PTR_RELEASE(BinaryData);

    %typemap(javacode) SearchResultSet %{
        public void SetDataAsArray(byte[] bytes)
        {
            //BinaryDataAPtr binaryData(new BinaryData(bytes, bytes.length));
            SetInputData(new BinaryData(bytes, bytes.length));
        }
    %}
#endif

#ifdef SWIGPHP
    class BinaryData
    {
      public:
            BinaryData(unsigned char *buffer, int length);
            int Size() const;
            std::string AsString() const;
            void Copy(unsigned char *buffer, int length) const; 
    };

    typedef std::auto_ptr<BinaryData> BinaryDataAPtr;
    SWIG_AUTO_PTR_RELEASE(BinaryData);
#endif

class SearchResultSet
{
  public:
    bool HasNext();
    
    int GetCount();
    
    const std::vector<std::string> GetColumns();
    
    std::string GetString(int columnIndex);
    
    std::string GetString(std::string columnName);

    void SetEncoding(EncodingType encoding);

    EncodingType GetEncoding();

    bool HasMaxRows();

    int GetReplyCode();

    std::string GetReplyText();

#if defined(SWIGCSHARP) || defined(SWIGJAVA) || defined(SWIGPHP)
    void SetInputData(BinaryData binaryData);
#endif

#ifdef SWIGPHP
    %extend {
        void SetDataAsString(std::string buffer)
        {
            BinaryData binaryData(buffer.data(), buffer.length());
            self->SetInputData(binaryData);
        }
    }
#endif
};
typedef std::auto_ptr<SearchResultSet> SearchResultSetAPtr;
#ifndef SWIGPHP
%template(SearchResultSetAPtr) std::auto_ptr<SearchResultSet>;
#endif

SWIG_AUTO_PTR_RELEASE(SearchRequest);
SWIG_AUTO_PTR_RELEASE(SearchResultSet);

class UpdateRequest 
{
  public:
    UpdateRequest(std::string resourceName, std::string className);
    
    std::string GetDelimiter();
    
    void SetDelimiter(std::string delimiter);
    
    StringVector GetAllFields();
    
    std::string GetField(std::string key);
    
    void SetField(std::string key, std::string value);
    
    static const int UPDATE_OK = 0;
    static const int AUTO_POPULATE = 1;
    static const int VALIDATE_ONLY = 2;

    std::string GetUpdateType();
    
    void SetUpdateType(std::string updateType);
    
    int GetValidateFlag();
    
    void SetValidateFlag(int flag);
    
    const StringVector GetAllWarnings();
    
    std::string GetWarningResponse(int warningNumber);

    std::string GetWarningResponse(std::string warningNumber);
    
    void SetWarningResponse(int warningNumber, std::string userResponse);
    
    void SetWarningResponse(std::string warningNumber, std::string userResponse);
};
typedef std::auto_ptr<UpdateRequest> UpdateRequestAPtr;
#ifndef SWIGPHP
%template(UpdateRequestAPtr) std::auto_ptr<UpdateRequest>;
#endif

class UpdateResponse 
{
  public:
    UpdateResponse();

    virtual ~UpdateResponse();

    StringVector GetColumns();
  
    EncodingType GetEncoding();
    
    std::string GetErrorFieldName();
    
    int GetErrorNumber();
    
    int GetErrorOffset();
    
    std::string GetErrorText();
    
    int GetReplyCode();
    
    std::string GetReplyText();
        
    std::string GetString(int columnIndex);

    std::string GetString(std::string columnName);

    std::string GetWarningFieldName();
    
    int GetWarningNumber();
    
    int GetWarningOffset();
    
    int GetWarningResponseRequired();
    
    std::string GetWarningText();
    
    bool HasNext();
    
    bool HasNextError();

    bool HasNextWarning();
    
    void SetEncoding(EncodingType encoding);
};

typedef std::auto_ptr<UpdateResponse> UpdateResponseAPtr;
#ifndef SWIGPHP
%template(UpdateResponseAPtr) std::auto_ptr<UpdateResponse>;
#endif

SWIG_AUTO_PTR_RELEASE(UpdateRequest);
SWIG_AUTO_PTR_RELEASE(UpdateResponse);

class LogoutResponse
{
  public:
    std::string GetBillingInfo();
    std::string GetLogoutMessage();
    int GetConnectTime();
};
typedef std::auto_ptr<LogoutResponse> LogoutResponseAPtr;
#ifndef SWIGPHP
%template(LogoutResponseAPtr) std::auto_ptr<LogoutResponse>;
#endif

SWIG_AUTO_PTR_RELEASE(LogoutResponse);

/****************************************************************************
 * GetObject
 ***************************************************************************/

typedef std::auto_ptr<InputStreamBridge> InputStreamBridgeAPtr;
SWIG_AUTO_PTR_RELEASE(InputStreamBridge);


class GetObjectRequest
{
  public:
%immutable;
    static const char * PHOTO_TYPE;
    static const char * PLAT_TYPE;
    static const char * VIDEO_TYPE;
    static const char * AUDIO_TYPE;
    static const char * THUMBNAIL_TYPE;
    static const char * MAP_TYPE;
    static const char * VRIMAGE_TYPE;
%mutable;

    GetObjectRequest(std::string resource, std::string type);
    
    bool GetLocation() const;
    
    void SetLocation(bool location);
    
    void AddObject(std::string resourceEntity, int objectId);
    
    void AddAllObjects(std::string resourceEntity);

    void SetObjectData(std::string objectData);

    void SetIgnoreMalformedHeaders(bool ignore);
};

class ObjectData
{
  public:

    std::string GetUID() const;

    std::string GetObjectType() const;

    std::string GetResourceName() const;

    std::string GetResourceID() const;

    std::string GetObjectID() const;

    std::string GetMimeType() const;

    bool GetIsDefault() const;

    std::string GetObjectModificationTimestamp() const;

    std::string GetModificationTimestamp() const;

    std::string GetOrderHint() const;

    std::string GetDescription() const;

    std::string GetCaption() const;

    std::string GetFileSize() const;

    std::string GetWidthPix() const;

    std::string GetHeightPix() const;

    std::string GetDuration() const;

    std::string GetWidthInch() const;

    std::string GetHeightInch() const;

    std::string GetValue(std::string key) const;
};

/*
 * Handling of the binary object data is a bit tricky.  Each language has
 * its own issues.
 *
 * Ruby
 * ====
 *
 * In Ruby, characters are 8-bits and strings may contain null characters.
 * This is very similar to C++ strings, so the binary data can be retrieved
 * with std::string.
 *
 * C#
 * ==
 *
 * In C#, characters are 16-bits, so strings cannot be used to represent
 * binary data.  C# does have a byte type, and a byte array (byte[]) can
 * be used to represent binary data.  The object data stream is also
 * represented as a native C# stream, using an intermediate bridge class
 * (InputStreamBridge).
 */
 
 
#ifdef SWIGCSHARP

%nodefault InputStreamBridge;
class InputStreamBridge
{
  public:
    int readByte() const;
    std::size_t read(unsigned char buffer[], std::size_t offset, std::size_t length) const; 
};

class BinaryData
{
  public:
        BinaryData(unsigned char buffer[], int length);
        int Size() const;
        std::string AsString() const;
        const char * AsChar() const;
        void Copy(unsigned char buffer[], int length) const; 
};
typedef std::auto_ptr<BinaryData> BinaryDataAPtr;
SWIG_AUTO_PTR_RELEASE(BinaryData);

%typemap(cscode) ObjectDescriptor %{
    public byte[] GetDataAsBytes()
    {
        BinaryData binaryData = GetData();
        int length = binaryData.Size();
        byte[] bytes = new byte[length];
        binaryData.Copy(bytes, bytes.Length);
        return bytes;
    }
    
    public System.IO.Stream GetDataStream()
    {
        return new CppInputStream(GetDataStreamBridge());
    }
%}

#endif

#ifdef SWIGJAVA
    %typemap(javacode) ObjectDescriptor %{
        public byte [] GetDataAsBytes()
        {
            BinaryData binaryData = GetData();
            int length = binaryData.Size();
            byte[] bytes = new byte[length];
            binaryData.Copy(bytes, length);
            return bytes;
        }
    %}
#endif

#ifdef SWIGPYTHON
    %typemap(out) BinaryDataAPtr (const char* s) %{
        s = ($1)->AsChar();
        $result = PyBytes_FromStringAndSize(s, ($1)->Size());
    %}
#endif

#ifdef SWIGJAVASCRIPT
    %typemap(out) BinaryDataAPtr (int size,
                                  const char* s,
                                  v8::Isolate* isolate,
                                  v8::Handle<v8::Uint8Array> array) %{
        s = ($1)->AsChar();
        size = ($1)->Size();
        isolate = v8::Isolate::GetCurrent();

        // Create an empty Uint8Array
        array = v8::Uint8Array::New(v8::ArrayBuffer::New(isolate, size),
                                    0, size);

        // Walk the array and copy the values into the javascript
        // managed array.  Its not super efficient, but its avoids
        // memory leaks of other methods.
        for (int i = 0; i < size; i++)
        {
            array->Set(i, v8::Integer::NewFromUnsigned(isolate, s[i]));
        }

        $result = array;
    %}
#endif

class ObjectDescriptor
{
  public:
    std::string GetObjectKey() const;
    
    int GetObjectId() const;
    
    std::string GetDescription() const;
    
    std::string GetLocationUrl() const;
    
    std::string GetContentType() const;
    
    ObjectData& GetObjectData();

    std::string GetSubDescription() const;

    std::string GetUID() const;
   
    int GetRetsReplyCode() const;

    std::string GetRetsReplyText() const;

    bool GetWildIndicator() const;

    bool GetPreferred() const;

#if defined(SWIGRUBY) || defined(SWIGPYTHON) || defined(SWIGPHP) || defined(SWIGPERL)
    %extend {
        std::string GetDataAsString()
        {
            istreamPtr inputStream = self->GetDataStream();
            return readIntoString(inputStream);
        }
    }
#endif

#if defined(SWIGJAVA) || defined(SWIGPYTHON) || defined(SWIGJAVASCRIPT)
    BinaryDataAPtr GetData();
#endif
    
#ifdef SWIGCSHARP
    BinaryDataAPtr GetData();

    %extend {
        InputStreamBridgeAPtr GetDataStreamBridge()
        {
            InputStreamBridgeAPtr streamBridge(
                new InputStreamBridge(self->GetDataStream()));
            return streamBridge;
        }
    }
#endif
};


#ifdef SWIGCSHARP

%typemap(csimports) GetObjectResponse %{
using System;
using System.Runtime.InteropServices;
using System.Collections;
%};
%typemap(csbase) GetObjectResponse "IEnumerable";

%typemap(cscode) GetObjectResponse %{
    public IEnumerator GetEnumerator()
    {
        return new ObjectDescriptorEnumerator(this);
    }
%}

#endif


class GetObjectResponse
{
  public:
    ObjectDescriptor * NextObject();
    int GetHttpResponse();
    std::string GetErrorText();
    
};
typedef std::auto_ptr<GetObjectResponse> GetObjectResponseAPtr;

SWIG_AUTO_PTR_RELEASE(GetObjectResponse);

/****************************************************************************
 * Metadata
 ***************************************************************************/
 

class MetadataElement
{
  public:
    enum MetadataType
    {
        /** System metadata. */
        SYSTEM,
        /** Resource metadata. */
        RESOURCE,
        /** Class metadata. */
        CLASS,
        /** Table metadata. */
        TABLE,
        /** Update metadata. */
        UPDATE,
        /** Update type metadata. */
        UPDATE_TYPE,
        /** Object metadata. */
        OBJECT,
        /** Search help metadata. */
        SEARCH_HELP,
        /** Edit mask metadata. */
        EDIT_MASK,
        /** Lookup metadata. */
        LOOKUP,
        /** Lookup type metadata. */
        LOOKUP_TYPE,
        /** Update help metadata. */
        UPDATE_HELP,
        /** Validation lookup metadata. */
        VALIDATION_LOOKUP,
        /** Validation lookup type metadata. */
        VALIDATION_LOOKUP_TYPE,
        /** Validation external metadata. */
        VALIDATION_EXTERNAL,
        /** Validation external type metadata. */
        VALIDATION_EXTERNAL_TYPE,
        /** Validation expression metadata. */
        VALIDATION_EXPRESSION,
        /** Foreign key metadata. */
        FOREIGN_KEY
    };
    typedef MetadataType Type;

    %rename(GetMetadataType) GetType() const;
    virtual MetadataType GetType() const = 0;
    
    StringVector GetAttributeNames() const;

    std::string GetStringAttribute(std::string attributeName,
                                   std::string defaultValue = "") const;
   
    int GetIntAttribute(std::string attributeName, int defaultValue = 0) const;
    
    bool GetBoolAttribute(std::string attributeName, bool defaultValue = false)
        const;

    virtual std::string GetId() const;

    std::string GetLevel() const;

    std::string GetPath() const;
};

class MetadataSystem : public MetadataElement
{
  public:
    std::string GetSystemID() const;  
    std::string GetSystemDescription() const;
    std::string GetComments() const;
    MetadataType GetType() const;
    std::string GetTimeZoneOffset() const;
    std::string GetMetadataID() const;
    std::string GetResourceVersion() const;
    std::string GetResourceDate() const;
    std::string GetForeignKeyVersion() const;
    std::string GetForeignKeyDate() const;
    std::string GetFilterVersion() const;
    std::string GetFilterDate() const;
};

class MetadataForeignKey : public MetadataElement
{
  public:
    std::string GetID() const;
    std::string GetForeignKeyID() const;
    std::string GetParentResourceID() const;
    std::string GetParentClassID() const;
    std::string GetParentSystemName() const;
    std::string GetChildResourceID() const;
    std::string GetChildClassID() const;
    std::string GetChildSystemName() const;
    std::string GetConditionalParentField() const;
    std::string GetConditionalParentValue() const;
    MetadataType GetType() const;
    bool GetOneToManyFlag() const;
};

typedef std::vector<MetadataForeignKey *> MetadataForeignKeyList;
%template(MetadataForeignKeyList) std::vector<MetadataForeignKey *>;

class MetadataResource : public MetadataElement
{
  public:
    std::string GetId() const;
    std::string GetResourceID() const;
    std::string GetStandardName() const;
    std::string GetKeyField() const;
    MetadataType GetType() const;
};

typedef std::vector<MetadataResource *> MetadataResourceList;
%template(MetadataResourceList) std::vector<MetadataResource *>;

class MetadataClass : public MetadataElement
{
  public:
    std::string GetId() const;
    std::string GetClassName() const;
    std::string GetStandardName() const;
    std::string GetDescription() const;
    MetadataType GetType() const;
    std::string GetVisibleName() const;
    std::string GetTableVersion() const;
    std::string GetTableDate() const;
    std::string GetUpdateVersion() const;
    std::string GetUpdateDate() const;
    std::string GetClassTimeStamp() const;
    std::string GetDeletedFlagField() const;
    std::string GetDeletedFlagValue() const;
    bool HasKeyIndex() const;
    bool HasOffsetSupport() const;
};

typedef std::vector<MetadataClass *> MetadataClassList;
%template(MetadataClassList) std::vector<MetadataClass *>;

class MetadataTable : public MetadataElement
{
  public:
    enum DataType
    {
        BOOLEAN,
        CHARACTER,
        DATE,
        DATE_TIME,
        TIME,
        TINY,
        SMALL,
        INT,
        LONG,
        DECIMAL
    };

    enum Interpretation
    {
        NUMBER,
        CURRENCY,
        LOOKUP,
        LOOKUP_MULTI,
        LOOKUP_BITSTRING,
        LOOKUP_BITMASK,
        NO_INTERPRETATION
    };
    
    enum Alignment
    {
        LEFT,
        RIGHT,
        CENTER,
        JUSTIFY,
        NO_ALIGNMENT
    };
    
    enum Units
    {
        FEET,
        METERS,
        SQUARE_FEET,
        SQUARE_METERS,
        ACRES,
        HECTARES,
        NO_UNITS
    };

    enum CharacterCase
    {
        UPPER,
        LOWER,
        EXACT,
        MIXED,
        NO_CASE
    };

    std::string GetSystemName() const;
    std::string GetStandardName() const;
    std::string GetLongName() const;
    std::string GetDBName() const;
    std::string GetShortName() const;
    int GetMaximumLength() const;
    DataType GetDataType() const;
    int GetPrecision() const;
    bool IsSearchable() const;
    Interpretation GetInterpretation() const;
    Alignment GetAlignment() const;
    bool UseSeparator() const;
    std::string GetEditMaskId() const;
    std::string GetLookupName() const;
    int GetMaxSelect() const;
    Units GetUnits() const;
    int GetIndex() const;
    int GetMinimum() const;
    std::string GetMinimumAsString() const;
    int GetMaximum() const;
    std::string GetMaximumAsString() const;
    int GetDefault() const;
    int GetRequired() const;
    std::string GetSearchHelpId() const;
    MetadataType GetType() const;
    bool IsUnique() const;
    bool InKeyIndex() const;
    std::string GetFilterParentField() const;
    int GetDefaultSearchOrder() const;
    CharacterCase GetCase() const;
};

typedef std::vector<MetadataTable *> MetadataTableList;
%template(MetadataTableList) std::vector<MetadataTable *>;

class MetadataLookup : public MetadataElement
{
  public:
    std::string GetId() const;
    std::string GetLookupName() const;
    std::string GetVisibleName() const;
    std::string GetVersion() const;
    std::string GetDate() const;
    MetadataType GetType() const;
    std::string GetFilterID() const;
    bool GetNotShownByDefault() const;
};

typedef std::vector<MetadataLookup *> MetadataLookupList;
%template(MetadataLookupList) std::vector<MetadataLookup *>;

class MetadataLookupType : public MetadataElement
{
  public:
    std::string GetId() const;
    std::string GetValue() const;
    std::string GetLongValue() const;
    std::string GetShortValue() const;
    MetadataType GetType() const;
};

typedef std::vector<MetadataLookupType *> MetadataLookupTypeList;
%template(MetadataLookupTypeList) std::vector<MetadataLookupType *>;

class MetadataObject : public MetadataElement
{
  public:
    std::string GetId() const;
    std::string GetObjectType() const;
    std::string GetMIMEType() const;
    std::string GetVisibleName() const;
    std::string GetDescription() const;
    MetadataType GetType() const;
    std::string GetObjectTimeStamp() const;
    std::string GetObjectCount() const;
    bool GetLocationAvailability() const;
    std::string GetObjectData() const;
    int GetMaxFileSize() const;
};

typedef std::vector<MetadataObject *> MetadataObjectList;
%template(MetadataObjectList) std::vector<MetadataObject *>;

class MetadataSearchHelp : public MetadataElement
{
  public:
    std::string GetId() const;
    std::string GetSearchHelpID() const;
    std::string GetValue() const;
    MetadataType GetType() const;
};

typedef std::vector<MetadataSearchHelp *> MetadataSearchHelpList;
%template(MetadataSearchHelpList) std::vector<MetadataSearchHelp *>;

class MetadataColumnGroup : public MetadataElement
{
  public:
    MetadataType GetType() const;
    std::string GetId() const;
    std::string GetColumnGroupName() const;
    std::string GetControlSystemName() const;
    std::string GetLongName() const;
    std::string GetShortName() const;
    std::string GetDescription() const;
};
                                                                                                                                                                               
typedef std::vector<MetadataColumnGroup *> MetadataColumnGroupList;
%template(MetadataColumnGroupList) std::vector<MetadataColumnGroup *>;

class MetadataColumnGroupControl : public MetadataElement
{
  public:
    MetadataType GetType() const;
    std::string GetId() const;
    int GetLowValue() const;
    int GetHighValue() const;
};

typedef std::vector<MetadataColumnGroupControl *> MetadataColumnGroupControlList;
%template(MetadataColumnGrouptControlList) std::vector<MetadataColumnGroupControl *>;

class MetadataColumnGroupNormalization : public MetadataElement
{
  public:
    MetadataType GetType() const;
    std::string GetId() const;
    std::string GetTypeIdentifier() const;
    int GetSequence() const;
    std::string GetColumnLabel() const;
    std::string GetSystemName() const;    
};

typedef std::vector<MetadataColumnGroupNormalization *> MetadataColumnGroupNormalizationList;
%template(MetadataColumnGroupNormalizationList) std::vector<MetadataColumnGroupNormalization *>;

class MetadataColumnGroupSet : public MetadataElement
{
  public:
    enum PresentationStyle
    {
      EDIT,
      MATRIX,
      LIST,
      EDIT_LIST,
      GIS_MAP_SEARCH,
      URL,
      NO_PRESENTATION
    };
    MetadataType GetType() const;
    std::string GetId() const;
    std::string GetColumnGroupSetName() const;
    std::string GetColumnGroupSetParent() const;
    int GetSequence() const;
    std::string GetLongName() const;
    std::string GetShortName() const;
    std::string GetDescription() const;
    std::string GetColumnGroupName() const;
    PresentationStyle GetPresentationStyle() const;
    std::string GetURL() const;
    std::string GetForeignKeyID() const;
};

typedef std::vector<MetadataColumnGroupSet *> MetadataColumnGroupSetList;
%template(MetadataColumnGroupSetList) std::vector<MetadataColumnGroupSet *>;

class MetadataColumnGroupTable : public MetadataElement
{
  public:
    MetadataType GetType() const;
    std::string GetId() const;
    std::string GetSystemName() const;
    std::string GetColumnGroupSetName() const;
    std::string GetLongName() const;
    std::string GetShortName() const;
    int GetDisplayOrder() const;
    int GetDisplayLength() const;
    int GetDisplayHeight() const;
    bool GetImmediateRefresh() const;
};

typedef std::vector<MetadataColumnGroupTable *> MetadataColumnGroupTableList;
%template(MetadataColumnGroupTableList) std::vector<MetadataColumnGroupTable *>;

class MetadataFilter : public MetadataElement
{
  public:
    ~MetadataFilter();
    MetadataType GetType() const;
    std::string GetID() const;
    std::string GetFilterID() const;
    std::string GetParentResourceID() const;
    std::string GetParentLookupName() const;
    std::string GetChildResourceID() const;
    std::string GetChildLookupName() const;
    bool GetNotShownByDefault() const;
};

typedef std::vector<MetadataFilter *> MetadataFilterList;
%template(MetadataFilterList) std::vector<MetadataFilter *>;

class MetadataFilterType : public MetadataElement
{
  public:
    ~MetadataFilterType();
    MetadataType GetType() const;
    std::string GetID() const;
    std::string GetFilterTypeID() const;
    std::string GetParentValue() const;
    std::string GetChildValue() const;
};

typedef std::vector<MetadataFilterType *> MetadataFilterTypeList;
%template(MetadataFilterTypeList) std::vector<MetadataFilterType *>;

%nodefault;

#ifdef SWIGJAVA
    %typemap(javacode) RetsMetadata %{
        public static RetsMetadata CreateMetadataFromArray(byte[] bytes)
        {
            return CreateAndParse(new BinaryData(bytes, bytes.length));
        }
    %}
#endif

class RetsMetadata
{
  public:
    MetadataSystem * GetSystem() const;

    MetadataForeignKeyList GetAllForeignKeys() const;

    MetadataResourceList GetAllResources() const;
    MetadataResource * GetResource(std::string resourceName) const;

    MetadataClassList GetAllClasses(std::string resourceName) const;
    MetadataClass * GetClass(std::string resourceName,
        std::string className) const;

    MetadataTableList GetAllTables(MetadataClass * metadataClass) const;

    MetadataTableList GetAllTables(std::string resourceName,
        std::string className) const;

    MetadataTable * GetTable(std::string resourceName, std::string className,
        std::string tableName) const;

    MetadataLookupList GetAllLookups(std::string resourceName) const;

    MetadataLookup * GetLookup(std::string resourceName,
        std::string lookupName) const;

    MetadataLookupTypeList GetAllLookupTypes(std::string resrouceName,
        std::string lookupName) const;

    MetadataLookupTypeList GetAllLookupTypes(MetadataLookup * metadataLookup)
      const;

    MetadataLookupType * GetLookupType(std::string resourceName,
        std::string lookupName,
        std::string lookupValue) const;

    MetadataObjectList GetAllObjects(std::string resourceName) const;

    MetadataObjectList GetAllObjects(MetadataResource * metadataResource)
      const;

    MetadataSearchHelp * GetSearchHelp(std::string resourceName,
        std::string searchHelpID) const;

    MetadataColumnGroupList GetAllColumnGroups(std::string resourceName,
        std::string className) const;

    MetadataColumnGroupSetList GetAllColumnGroupSets(MetadataClass * metadataClass) const;

    MetadataColumnGroupSetList GetAllColumnGroupSets(std::string resourceName,
        std::string className) const;

    MetadataColumnGroupControlList GetAllColumnGroupControls(MetadataClass * metadataClass,
        std::string columnGroup) const;

    MetadataColumnGroupControlList GetAllColumnGroupControls(std::string resourceName,
        std::string className,
        std::string columnGroup) const;

    MetadataColumnGroupTableList GetAllColumnGroupTables(MetadataClass * metadataClass,
        std::string columnGroup) const;

    MetadataColumnGroupTableList GetAllColumnGroupTables(std::string resourceName,
        std::string className,
        std::string columnGroup) const;
    MetadataColumnGroupNormalizationList GetAllColumnGroupNormalizations(MetadataClass * metadataClass,
        std::string columnGroup) const;

    MetadataColumnGroupNormalizationList GetAllColumnGroupNormalizations(std::string resourceName,
        std::string className,
        std::string columnGroup) const;

    MetadataFilterList GetAllFilters() const;
        
    MetadataFilterTypeList GetAllFilterTypes(std::string filter) const;


#if defined(SWIGJAVA) || defined(SWIGPHP)
    static RetsMetadata * CreateAndParse(BinaryData binaryData,
        EncodingType encoding = EncodingType::RETS_XML_DEFAULT_ENCODING,
        ExceptionErrorHandler * handler = ExceptionErrorHandler::GetInstance());
#if defined(SWIGPHP)
    %extend {
      static RetsMetadata * CreateMetadataFromString(std::string bytes)
      {
        BinaryData binaryData(bytes.data(), bytes.length());
        return RetsMetadata::CreateAndParse(binaryData);
      }
    }
#endif

#endif
};

%default;
 
/****************************************************************************
 * RetsSession
 ***************************************************************************/

enum RetsVersion
{
    RETS_1_0,
    RETS_1_5,
    RETS_1_7,
    RETS_1_7_2,
    RETS_1_8,
    RETS_1_8_0
};

enum EncodingType
{
    RETS_XML_DEFAULT_ENCODING,
    RETS_XML_ISO_ENCODING,
    RETS_XML_UTF8_ENCODING
};

enum UserAgentAuthType
{
    USER_AGENT_AUTH_RETS_1_7,
    USER_AGENT_AUTH_INTEREALTY,
};


#ifdef SWIGCSHARP
// Right now, we have a hand rolled delegate for logging in CSharp.
// Eventually we'll move this over to using directors.
%typemap(cscode) RetsHttpLogger %{
    public delegate void Delegate(Type type, byte[] data);
    public delegate void NativeDelegate(Type type, System.IntPtr data, int length);
%}
#endif

// For right now, a director for RetsHttpLogger has only been defined
// for ruby.  This also adds some ruby GC magic to make sure the director
// class isn't cleaned up before RetsSession.
#ifdef SWIGRUBY
%trackobjects;
%markfunc RetsSession "mark_RetsSession";

%header %{
static void mark_RetsSession(void* ptr)
{
    RetsSession* session = (RetsSession*) ptr;

    RetsHttpLogger* logger = session->GetHttpLogger();

    VALUE object = SWIG_RubyInstanceFor(logger);

    if (object != Qnil) {
        rb_gc_mark(object);
    }
}
%}
#endif

#if defined(SWIGRUBY) || defined(SWIGPYTHON) || defined(SWIGJAVA)
%feature("director") RetsHttpLogger;
#endif

class RetsHttpLogger
{
  public:
    enum Type
    {
        RECEIVED,
        SENT,
        INFORMATIONAL
    };

    virtual ~RetsHttpLogger();

    virtual void logHttpData(Type type, std::string data) = 0;
};

#ifdef SWIGCSHARP
// The following few classes were added to see if we can get C# to
// call out to the translator.  This is commented out as its not ready
// for release as of yet.  When ready, also uncomment out the director
// line on top.
// 
%feature("director") SqlMetadata;
class SqlMetadata
{
  public:
    virtual ~SqlMetadata() { };

    virtual bool IsLookupColumn(std::string tableName, std::string columnName)
        = 0;
};

%typemap(cscode) SqlMetadata %{
      public SqlMetadataPtr metadataptr_reference;
      public SqlToDmqlCompilerPtr compiler_reference;

    public addCompilerReference (SlToDmqlCompilerPtr compiler_ptr)
    {
      compiler_reference = compiler_ptr;
    }
%}

class DmqlCriterion 
{
  public:
    virtual ~DmqlCriterion();

    virtual std::ostream & ToDmql(std::ostream & outputStream) const = 0;

    std::string ToDmqlString();
};

class DmqlQuery 
{
  public:
    DmqlQuery();
    virtual ~DmqlQuery();

    std::string GetResource() const;

    void SetResource(std::string resource);

    std::string GetClass() const;

    void SetClass(std::string aClass);

    /**
     * Returns a pointer to a StringVector.  DmqlQuery is responsible for
     * freeing the object.
     */
    StringVector * GetFieldsPtr() const;

    StringVectorPtr GetFields() const;

    void AddField(std::string column);

    /**
     * Returns a pointer to a DmqlCriterion.  DmqlQuery is responsible for
     * freeing the object.
     */
    DmqlCriterion * GetCriterionPtr() const;

    DmqlCriterionPtr GetCriterion() const;

    void SetCriterion(DmqlCriterionPtr criterion);

    int GetLimit() const;

    void SetLimit(int limit);

    int GetOffset() const;

    void SetOffset(int offset);

    SearchRequest::CountType GetCountType() const;

    void SetCountType(SearchRequest::CountType countType);

    virtual std::ostream & Print(std::ostream & outputStream) const;
};

class GetObjectQuery
{
    GetObjectQuery();
    
    std::string GetResource() const;
    
    void SetResource(std::string resource);
    
    std::string GetType() const;
    
    void SetType(std::string type);
    
    std::string GetObjectKey() const;
    
    void SetObjectKey(std::string objectKey);
    
    IntVectorPtr GetObjectIds() const;

    /**
     * Returns a pointer to the IntVector.  GetObjectQuery is
     * responsiblity for cleaning up the object.
     */
    IntVector * GetObjectIdsPtr() const;
    
    void AddObjectId(int objectId);

    bool GetUseLocation() const;

    void SetUseLocation(bool useLocation);

    virtual std::ostream & Print(std::ostream & outputStream) const;

    /**
     * When you see this value in the ObjectIds, it means that it is a
     * place holder value that should be replaced.
     */
    static const int SUBSTITUTE_VALUE = -99;
};

class LookupQuery
{
  public:
    LookupQuery();
    virtual ~LookupQuery();

    std::string GetResource() const;

    void SetResource(std::string resource);

    std::string GetLookup() const;

    void SetLookup(std::string lookup);
};

class LookupColumnsQuery
{
  public:
    LookupColumnsQuery();
    virtual ~LookupColumnsQuery();

    std::string GetResource() const;

    void SetResource(std::string resource);

    std::string GetClass() const;

    void SetClass(std::string aClass);
};

typedef boost::shared_ptr<DmqlCriterion> DmqlCriterionPtr;
%template(DmqlCriterionPtr) boost::shared_ptr<DmqlCriterion>;

typedef boost::shared_ptr<DmqlQuery> DmqlQueryPtr;
%template(DmqlQueryPtr) boost::shared_ptr<DmqlQuery>;

typedef boost::shared_ptr<GetObjectQuery> GetObjectQueryPtr;
%template(GetObjectQueryPtr) boost::shared_ptr<GetObjectQuery>;

typedef boost::shared_ptr<LookupQuery> LookupQueryPtr;
%template(LookupQueryPtr) boost::shared_ptr<LookupQuery>;

typedef boost::shared_ptr<LookupColumnsQuery> LookupColumnsQueryPtr;
%template(LookupColumnsQueryPtr) boost::shared_ptr<LookupColumnsQuery>;

typedef boost::shared_ptr<SqlMetadata> SqlMetadataPtr;
%template(SqlMetadataPtr) boost::shared_ptr<SqlMetadata>;

/*
 * I can not figure out how to get SWIG to add this to the shared_ptr wrapper. The shared_ptr 
 * needs to reference SqlMetatadata, otherwise it is getting garbage collected before the
 * shared_ptr is released.
 */
//%typemap(cscode) SqlMetadataPtr  %{
//    public SqlMetadata metadata_reference = null;
//
//    public AddReference(SqlMetadata metadata)
//    {
//      metadata_reference = metadata;
//    }
//%}


typedef boost::shared_ptr<SqlToDmqlCompiler> SqlToDmqlCompilerPtr;
%template(SqlToDmqlCompilerPtr) boost::shared_ptr<SqlToDmqlCompiler>;

%typemap(cscode) SqlToDmqlCompiler %{
      private SqlMetadata metadata_reference = null;
      private SqlMetadataPtr metadataptr_reference = null;

    /*
     * Add a layer of indirection to attempt to reference the objects so they do not get
     * garbage collected. This too appears to fail. When in doubt, use brute force.
     */
    public SqlToDmqlCompiler(SqlMetadataPtr metadata_ptr, SqlMetadata metadata) : this(metadata_ptr)
    {
      metadataptr_reference = metadata_ptr;
      //metadataptr_reference.AddReference(metadata);
      /*
       * Since simply adding the reference doesn't appear to be stopping the garbage collector,
       * use brute force.
       */
      global::System.GC.SuppressFinalize(metadata_ptr);
    }

    /*
     * Wrap the metadata into a metadata pointer.
     */
    public SqlToDmqlCompiler(SqlMetadata metadata) : this(new SqlMetadataPtr(metadata), metadata) 
    {
      metadata_reference = metadata;
      /*
       * Since simply adding the reference doesn't appear to be stopping the garbage collector,
       * use brute force.
       */
      global::System.GC.SuppressFinalize(metadata);
    }
%}
%typemap(csfinalize) SqlToDmqlCompiler %{
    ~SqlToDmqlCompiler() 
    {
      /*
       * Clean up for ourselves.
       */
      Dispose();
      /*
       * We know we created the metadataptr, so go ahead and dispose it as well.
       */
      if (metadataptr_reference != null)
        metadataptr_reference.Dispose();
      metadataptr_reference = null;
      /*
       * Something appears to be destructing the metadata elsewhere, so we do not want
       * it garbage collected until that can be figured out. I suspect there may be
       * a memory leak around the director stuff.
       */
      //if (metadata_reference != null)
      //  GC.ReRegisterForFinalize(metadata_reference);
      metadata_reference = null;
    }
%}

class SqlToDmqlCompiler
{
  public:
    SqlToDmqlCompiler(SqlMetadataPtr metadata);
    
    enum QueryType {DMQL_QUERY, GET_OBJECT_QUERY, LOOKUP_QUERY,
                    LOOKUP_COLUMNS_QUERY};
    
    QueryType sqlToDmql(std::string sql);

    DmqlQueryPtr GetDmqlQuery() const;
    
    GetObjectQueryPtr GetGetObjectQuery() const;

    LookupQueryPtr GetLookupQuery() const;

    LookupColumnsQueryPtr GetLookupColumnsQuery() const;
};

%typemap(ctype)  RetsHttpLoggerCallback "RetsHttpLoggerCallback"
%typemap(cstype) RetsHttpLoggerCallback "RetsHttpLogger.NativeDelegate"
%typemap(csin)   RetsHttpLoggerCallback "$csinput"
%typemap(imtype) RetsHttpLoggerCallback "RetsHttpLogger.NativeDelegate"
%typemap(in)     RetsHttpLoggerCallback {$1 = $input;}

typedef void (*RetsHttpLoggerCallback)(RetsHttpLogger::Type type,
    void * data, int length);

class RetsHttpLoggerBridge : public RetsHttpLogger
{
  public:
    RetsHttpLoggerBridge(RetsHttpLoggerCallback loggerCallback);
    void logHttpData(Type type, std::string data);
};

%csmethodmodifiers RetsSession::SetHttpLogger "private";

%typemap(cscode) RetsSession %{
    private RetsHttpLogger.Delegate mLoggerDelegate = null;
    private RetsHttpLogger.NativeDelegate mNativeDelegate = null;
    private RetsHttpLogger mLogger = null;

    public RetsHttpLogger.Delegate LoggerDelegate
    {
        get { return mLoggerDelegate; }
        set
        {
            mLoggerDelegate = value;

            if (mLoggerDelegate != null)
            {
                // Need to save references to both the delegate and
                // the logger otherwise they will be garbage collected
                // while still in use.
                mNativeDelegate = new RetsHttpLogger.NativeDelegate(
                    this.LoggerDelegateBridge);
                mLogger = new RetsHttpLoggerBridge(mNativeDelegate);
                SetHttpLogger(mLogger);
            }
            else
            {
                // Disconnect callback prior to making them available
                // for GC
                SetHttpLogger(null);
                mLogger = null;
                mNativeDelegate = null;
            }
        }
    }
    
    private void LoggerDelegateBridge(RetsHttpLogger.Type type, System.IntPtr data,
        int length)
    {
        byte[] byteData = new byte[length];
        System.Runtime.InteropServices.Marshal.Copy(data, byteData, 0, length);
        mLoggerDelegate(type, byteData);
    }

    public byte [] SearchAsArray(SearchRequest request)
    {
        BinaryData binaryData = Search_(request);
        int length = binaryData.Size();
        byte[] bytes = new byte[length];
        binaryData.Copy(bytes, length);
        return bytes;
    }
%}
#endif

#if defined(SWIGJAVA_prototype)
    // Prototype for returning the data to Java in a stream
    // Gotta adjust the in and argout typemaps for the StreamBridge.
    //
    %nodefault InputStreamBridge;
    class InputStreamBridge
    {
      public:
        InputStreamBridge(istreamPtr inputStream);
        int readByte() const;
        std::size_t read(unsigned char buffer[], std::size_t offset, std::size_t length) const; 
    };

    %typemap(javacode) RetsSession  %{
        public CppInputStream  SearchAsStream(SearchRequest request)
        {
            SWIGTYPE_p_istreamPtr  inputStream = SearchStream(request);

            //CppInputStream streamBridge = new IOStreamBridge(inputStream);;
            return new CppInputStream(new InputStreamBridge(inputStream));
            //return streamBridge;
        }
    %}
#endif // prototype code

#ifdef SWIGJAVA
    %typemap(javacode) RetsSession  %{
        public byte [] GetMetadataAsArray()
            throws RetsHttpException, RetsReplyException, RetsException
        {
            BinaryData binaryData = GetMetadata_();
            int length = binaryData.Size();
            byte[] bytes = new byte[length];
            binaryData.Copy(bytes, length);
            return bytes;
        }

        public byte [] SearchAsArray(SearchRequest request)
            throws RetsHttpException, RetsReplyException, RetsException
        {
            BinaryData binaryData = Search_(request);
            int length = binaryData.Size();
            byte[] bytes = new byte[length];
            binaryData.Copy(bytes, length);
            return bytes;
        }
    %}
#endif


class RetsSession
{
  public:
    static const unsigned int MODE_CACHE          = 0x01;
    static const unsigned int MODE_NO_STREAM      = 0x02;
    static const unsigned int MODE_NO_EXPECT      = 0x04;
    static const unsigned int MODE_NO_SSL_VERIFY  = 0x08;

    RetsSession(std::string loginUrl) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    ~RetsSession();

    std::string GetLoginUrl() const;

    bool Login(std::string userName, std::string password) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    LoginResponse * GetLoginResponse() const 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    
    CapabilityUrls * GetCapabilityUrls() const 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    std::string GetAction()       throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    SearchRequestAPtr CreateSearchRequest(std::string searchType, 
                                          std::string searchClass,
                                          std::string query) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    SearchResultSetAPtr Search(SearchRequest * request) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    
#if !defined(SWIGPHP)
    void Search(SearchRequest * request, std::ostream & outputStream) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
#endif
    
#if defined(SWIGJAVA) || defined(SWIGCSHARP) || defined(SWIGPHP)
    BinaryDataAPtr  Search_(SearchRequest * request) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
#endif
    
#if defined(SWIGJAVA_prototype)
    // Prototype for returning the data wrapped in a stream.
    istreamPtr SearchStream(SearchRequest * request) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
#endif
    
    LogoutResponseAPtr Logout() 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    
    RetsMetadata * GetMetadata() 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
#if !defined(SWIGPHP)
    void GetMetadata(std::ostream & outputStream) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
#endif
    
#if defined(SWIGJAVA) || defined(SWIGCSHARP)
    BinaryDataAPtr  GetMetadata_() 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
#endif
    
    bool IsIncrementalMetadata() const 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    
    void SetIncrementalMetadata(bool incrementalMetadata) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    
    GetObjectResponseAPtr GetObject(GetObjectRequest * request) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    void SetUserAgent(std::string userAgent) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    void UseHttpGet(bool useHttpGet) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    
    void SetRetsVersion(RetsVersion retsVersion) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    
    RetsVersion GetRetsVersion() const 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    
    RetsVersion GetDetectedRetsVersion() const 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    std::string RetsVersionToString(RetsVersion retsVersion)
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    void SetHttpLogger(RetsHttpLogger * logger) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    void SetUserAgentAuthType(UserAgentAuthType type) 
                                  throw(RetsHttpException,
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    UserAgentAuthType GetUserAgentAuthType() const 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    
    void SetUserAgentPassword(std::string userAgentPassword) 
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    void SetDefaultEncoding(EncodingType encoding)
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    void Cleanup()
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    void SetLogEverything(bool logging)
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    void SetModeFlags(unsigned int flags)
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    static std::string GetLibraryVersion()
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    void SetProxy(std::string url, std::string password)
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    void SetHttpLogName(std::string filename)
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    void SetTimeout(int seconds)
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

    UpdateRequestAPtr CreateUpdateRequest(std::string resourceName, 
                                          std::string className)
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);
    
    UpdateResponseAPtr Update(UpdateRequest * request)
                                  throw(RetsHttpException, 
                                        RetsReplyException,
                                        RetsException,
                                        std::exception);

#ifdef SWIGPHP
    %extend {
        std::string SearchAsString(SearchRequest * request)
        {
            BinaryDataAPtr binaryData = self->Search_(request);
            return binaryData->AsString();
        }

        std::string GetMetadataAsString()
        {
            BinaryDataAPtr binaryData = self->GetMetadata_();
            return binaryData->AsString();
        }
    }
#endif
};

/* Local Variables: */
/* mode: c++ */
/* End: */
