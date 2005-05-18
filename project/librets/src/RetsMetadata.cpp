#include <sstream>
#include "librets/RetsMetadata.h"
#include "librets/MetadataByLevelCollector.h"
#include "librets/MetadataSystem.h"
#include "librets/MetadataResource.h"
#include "librets/MetadataClass.h"
#include "librets/MetadataTable.h"
#include "librets/RetsException.h"

using namespace librets;
using std::string;
using std::vector;
using std::ostringstream;
namespace b = boost;

RetsMetadata::RetsMetadata(MetadataByLevelCollectorPtr collector)
{
    mCollector = collector;
    InitSystem();
    InitAllClasses();
}

void RetsMetadata::InitSystem()
{
    MetadataElementListPtr elements =
        mCollector->Find(MetadataElement::SYSTEM, "");
    if (elements->size() != 1)
    {
        ostringstream message;
        message << "Expecting 1 system metadata element, found "
                << elements->size();
        throw RetsException(message.str());
    }
    MetadataElementPtr element = elements->at(0);
    mSystem = b::dynamic_pointer_cast<MetadataSystem>(element);
}

void RetsMetadata::InitAllClasses()
{
    mAllClasses.reset(new MetadataClassList());
    MetadataElementListPtr resources =
        mCollector->Find(MetadataElement::RESOURCE, "");
    MetadataElementList::iterator i;
    for (i = resources->begin(); i != resources->end(); i++)
    {
        MetadataResourcePtr resource =
            b::dynamic_pointer_cast<MetadataResource>(*i);
        string level = resource->GetResourceID();

        MetadataElementListPtr classes =
            mCollector->Find(MetadataElement::CLASS, level);
        MetadataElementList::iterator j;
        for (j = classes->begin(); j != classes->end(); j++)
        {
            MetadataClassPtr aClass =
                b::dynamic_pointer_cast<MetadataClass>(*j);
            mAllClasses->push_back(aClass);
        }
    }
}

MetadataSystemPtr RetsMetadata::GetSystem() const
{
    return mSystem;
}

MetadataClassListPtr RetsMetadata::GetAllClasses() const
{
    return mAllClasses;
}

MetadataClassPtr RetsMetadata::GetClass(string resourceName, string className)
    const
{
    MetadataClassPtr metadataClass;
    return metadataClass;
}

MetadataTableListPtr RetsMetadata::GetTablesForClass(
    MetadataClassPtr metadataClass) const
{
    string level = metadataClass->GetLevel() + ":" +
        metadataClass->GetClassName();
    MetadataElementListPtr elements =
        mCollector->Find(MetadataElement::TABLE, level);

    MetadataTableListPtr tables(new MetadataTableList());
    MetadataElementList::iterator i;
    for (i = elements->begin(); i != elements->end(); i++)
    {
        MetadataTablePtr table = b::dynamic_pointer_cast<MetadataTable>(*i);
        tables->push_back(table);
    }
    return tables;
}
