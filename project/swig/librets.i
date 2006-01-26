%module librets

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

%template(StringVector) std::vector<std::string>;
%template(ByteVector) std::vector<unsigned char>;

%typemap(ctype)  unsigned char[] "unsigned char*"
%typemap(cstype) unsigned char[] "byte[]"
%typemap(csin)   unsigned char[] "$csinput"
%typemap(imtype) unsigned char[] "byte[]"
%typemap(in)     unsigned char[] {$1 = $input;}

#if 0
%typemap(ctype) unsigned char * "unsigned char *"
%typemap(imtype, out="byte[]") unsigned char * "byte[]"
%typemap(cstype) unsigned char * "byte[]"
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
 
class GetObjectRequest
{
  public:
    GetObjectRequest(std::string resource, std::string type);
    
    bool GetLocation() const;
    
    void SetLocation(bool location);
    
    void AddObject(std::string resourceEntity, int objectId);
    
    void AddAllObjects(std::string resourceEntity);
};

class BinaryData
{
  public:
  	int Size() const;
  	std::string AsString() const;
  	const char * AsChar() const;
  	void Copy(unsigned char buffer[], int length) const; 
  	void ReadToEof(std::istream & inputStream);
};
typedef std::auto_ptr<BinaryData> BinaryDataAPtr;
SWIG_AUTO_PTR_RELEASE(BinaryData);

%typemap(cscode) ObjectDescriptor %{
    public byte[] GetDataBytes()
    {
        BinaryData binaryData = GetData();
        int length = binaryData.Size();
        byte[] bytes = new byte[length];
        binaryData.Copy(bytes, bytes.Length);
        return bytes;
    }
%}


class ObjectDescriptor
{
  public:
    std::string GetObjectKey() const;
    
    int GetObjectId() const;
    
    std::string GetDescription() const;
    
    std::string GetLocationUrl() const;
    
    std::string GetContentType() const;
    
    BinaryDataAPtr GetData();
    
    %extend {
        std::string GetStringData()
        {
            istreamPtr inputStream = self->GetDataStream();
            return readIntoString(*inputStream);
        }
    }
};

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
 
class MetadataSystem
{
  public:
    std::string GetSystemID() const;  
    std::string GetSystemDescription() const;
    std::string GetComments() const;
};

class MetadataResource
{
  public:
    std::string GetId() const;
    std::string GetResourceID() const;
    std::string GetStandardName() const;
    std::string GetKeyField() const;
};

typedef std::vector<MetadataResource *> MetadataResourceList;
%template(MetadataResourceList) std::vector<MetadataResource *>;

class MetadataClass
{
  public:
    virtual std::string GetId() const;
    std::string GetClassName() const;
    std::string GetStandardName() const;
    std::string GetDescription() const;
};

typedef std::vector<MetadataClass *> MetadataClassList;
%template(MetadataClassList) std::vector<MetadataClass *>;

class MetadataTable
{
  public:
    std::string GetSystemName() const;
    std::string GetStandardName() const;
    std::string GetLongName() const;
};

typedef std::vector<MetadataTable *> MetadataTableList;
%template(MetadataTableList) std::vector<MetadataTable *>;

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

class RetsSession
{
  public:
    RetsSession(std::string loginUrl);

    bool Login(std::string userName, std::string password);

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
};


/* Local Variables: */
/* mode: c++ */
/* End: */
