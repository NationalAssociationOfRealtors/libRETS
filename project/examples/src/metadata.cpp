#include "librets.h"
#include <iostream>

using namespace librets;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::auto_ptr;

void dumpSystem(RetsMetadataPtr metadata);
void dumpAllClasses(RetsMetadataPtr metadata);
void dumpAllTables(RetsMetadataPtr metadata, MetadataClassPtr aClass);

int main(int argc, char * argv[])
{
    try
    {
        RetsSessionPtr session(
            new RetsSession("http://demo.crt.realtors.org:6103/rets/login"));
        session->Login("Joe", "Schmoe");

        RetsMetadataPtr metadata = session->GetMetadata();
        dumpSystem(metadata);
        dumpAllClasses(metadata);

        session->Logout();
    }
    catch (RetsException & e)
    {
        cout << "An error occured: " << e.GetMessage() << endl;
    }
}

void dumpSystem(RetsMetadataPtr metadata)
{
    MetadataSystemPtr system = metadata->GetSystem();
    cout << "Sytem ID: " << system->GetSystemID() << endl;
    cout << "Comments: " << system->GetComments() << endl;
}

void dumpAllClasses(RetsMetadataPtr metadata)
{
    MetadataClassListPtr classes = metadata->GetAllClasses();
    MetadataClassList::iterator i;
    for (i = classes->begin(); i != classes->end(); i++)
    {
        MetadataClassPtr aClass(*i);
        cout << "Class name: " << aClass->GetClassName() << endl;
        dumpAllTables(metadata, aClass);
    }
}

void dumpAllTables(RetsMetadataPtr metadata, MetadataClassPtr aClass)
{
    MetadataTableListPtr tables = metadata->GetTablesForClass(aClass);
    MetadataTableList::iterator i;
    for (i = tables->begin(); i != tables->end(); i++)
    {
        MetadataTablePtr table(*i);
        cout << "System name: " << table->GetSystemName() << endl;
    }
}
