%module librets


#ifdef SWIGCSHARP
%{
#include "librets_sharp.h"
%}
#endif

#ifdef SWIGRUBY
%{
#include "librets_ruby.h"
%}
#endif

%{
#include "librets.h"
#include <iostream>
#include <sstream>

using namespace librets;
using namespace librets::util;
using std::vector;
using std::string;
%}

#ifndef SWIGXML
%include "std_string.i"
%include "std_vector.i"
#endif
%include "exception.i"
%include "auto_ptr_release.i"

typedef std::vector<std::string> StringVector;
%template(StringVector) std::vector<std::string>;
%template(ByteVector) std::vector<unsigned char>;

#ifdef SWIGCSHARP
%typemap(ctype)  unsigned char[] "unsigned char*"
%typemap(cstype) unsigned char[] "byte[]"
%typemap(csin)   unsigned char[] "$csinput"
%typemap(imtype) unsigned char[] "byte[]"
%typemap(in)     unsigned char[] {$1 = $input;}
#endif

%typemap(out) std::vector<unsigned char>
{
    
}

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
    virtual std::string GetMessage();
};

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
};
typedef std::auto_ptr<SearchRequest> SearchRequestAPtr;

class SearchResultSet
{
  public:
    bool HasNext();
    
    int GetCount();
    
    const std::vector<std::string> GetColumns();
    
    std::string GetString(int columnIndex);
    
    std::string GetString(std::string columnName);
};
typedef std::auto_ptr<SearchResultSet> SearchResultSetAPtr;

class LogoutResponse
{
  public:
    std::string GetBillingInfo();
    std::string GetLogoutMessage();
    int GetConnectTime();
};
typedef std::auto_ptr<LogoutResponse> LogoutResponseAPtr;


SWIG_AUTO_PTR_RELEASE(SearchRequest);
SWIG_AUTO_PTR_RELEASE(SearchResultSet);
SWIG_AUTO_PTR_RELEASE(LogoutResponse);

/****************************************************************************
 * GetObject
 ***************************************************************************/

typedef std::auto_ptr<InputStreamBridge> InputStreamBridgeAPtr;
SWIG_AUTO_PTR_RELEASE(InputStreamBridge);


class GetObjectRequest
{
  public:
    GetObjectRequest(std::string resource, std::string type);
    
    bool GetLocation() const;
    
    void SetLocation(bool location);
    
    void AddObject(std::string resourceEntity, int objectId);
    
    void AddAllObjects(std::string resourceEntity);
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
    int read(unsigned char buffer[], int offset, int length) const; 
};

class BinaryData
{
  public:
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

class ObjectDescriptor
{
  public:
    std::string GetObjectKey() const;
    
    int GetObjectId() const;
    
    std::string GetDescription() const;
    
    std::string GetLocationUrl() const;
    
    std::string GetContentType() const;
    
#if defined(SWIGRUBY) || defined(SWIGPYTHON)
    %extend {
        std::string GetDataAsString()
        {
            std::istream * inputStream = self->GetDataStream();
            return readIntoString(*inputStream);
        }
    }
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


class GetObjectResponse
{
  public:
    ObjectDescriptor * NextObject();
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
};

class MetadataResource : public MetadataElement
{
  public:
    std::string GetId() const;
    std::string GetResourceID() const;
    std::string GetStandardName() const;
    std::string GetKeyField() const;
};

typedef std::vector<MetadataResource *> MetadataResourceList;
%template(MetadataResourceList) std::vector<MetadataResource *>;

class MetadataClass : public MetadataElement
{
  public:
    virtual std::string GetId() const;
    std::string GetClassName() const;
    std::string GetStandardName() const;
    std::string GetDescription() const;
};

typedef std::vector<MetadataClass *> MetadataClassList;
%template(MetadataClassList) std::vector<MetadataClass *>;

class MetadataTable : public MetadataElement
{
  public:
    std::string GetSystemName() const;
    std::string GetStandardName() const;
    std::string GetLongName() const;
};

typedef std::vector<MetadataTable *> MetadataTableList;
%template(MetadataTableList) std::vector<MetadataTable *>;

class MetadataLookup : public MetadataElement
{
  public:
    virtual std::string GetId() const;
    std::string GetLookupName() const;
    std::string GetVisibleName() const;
    std::string GetVersion() const;
    std::string GetDate() const;
};

typedef std::vector<MetadataLookup *> MetadataLookupList;
%template(MetadataLookupList) std::vector<MetadataLookup *>;

class MetadataLookupType : public MetadataElement
{
  public:
    virtual std::string GetId() const;
    std::string GetValue() const;
    std::string GetLongValue() const;
    std::string GetShortValue() const;
};

typedef std::vector<MetadataLookupType *> MetadataLookupTypeList;
%template(MetadataLookupTypeList) std::vector<MetadataLookupType *>;

%nodefault;

class RetsMetadata
{
  public:
    MetadataSystem * GetSystem() const;

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
};

%default;
 
/****************************************************************************
 * RetsSession
 ***************************************************************************/

enum RetsVersion
{
    RETS_1_0,
    RETS_1_5
};

enum UserAgentAuthType
{
    USER_AGENT_AUTH_RETS_1_7,
    USER_AGENT_AUTH_INTEREALTY,
};


%typemap(cscode) RetsHttpLogger %{
    public delegate void Delegate(Type type, byte[] data);
    public delegate void NativeDelegate(Type type, IntPtr data, int length);
%}

class RetsHttpLogger
{
  public:
    enum Type
    {
        RECEIVED,
        SENT,
        INFORMATIONAL
    };
    
    virtual void logHttpData(Type type, std::string data) = 0;
};


#ifdef SWIGCSHARP

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
    
    private void LoggerDelegateBridge(RetsHttpLogger.Type type, IntPtr data,
        int length)
    {
        byte[] byteData = new byte[length];
        System.Runtime.InteropServices.Marshal.Copy(data, byteData, 0, length);
        mLoggerDelegate(type, byteData);
    }
%}

#endif


class RetsSession
{
  public:
    RetsSession(std::string loginUrl);

    bool Login(std::string userName, std::string password);

    LoginResponse * GetLoginResponse() const;
    
    CapabilityUrls * GetCapabilityUrls() const;

    std::string GetAction();

    SearchRequestAPtr CreateSearchRequest(std::string searchType, 
                                          std::string searchClass,
                                          std::string query);

    SearchResultSetAPtr Search(SearchRequest * request);
    
    LogoutResponseAPtr Logout();
    
    RetsMetadata * GetMetadata();
    
    bool IsIncrementalMetadata() const;
    
    void SetIncrementalMetadata(bool incrementalMetadata);
    
    GetObjectResponseAPtr GetObject(GetObjectRequest * request);

    void SetUserAgent(std::string userAgent);

    void UseHttpGet(bool useHttpGet);
    
    void SetRetsVersion(RetsVersion retsVersion);
    
    RetsVersion GetRetsVersion() const;
    
    RetsVersion GetDetectedRetsVersion() const;
    
    void SetHttpLogger(RetsHttpLogger * logger);

    void SetUserAgentAuthType(UserAgentAuthType type);

    UserAgentAuthType GetUserAgentAuthType() const;
    
    void SetUserAgentPassword(std::string userAgentPassword);
};


/* Local Variables: */
/* mode: c++ */
/* End: */
