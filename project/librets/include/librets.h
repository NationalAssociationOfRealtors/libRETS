#ifndef LIBRETS_H
#define LIBRETS_H

/**
 * @mainpage librets: A C++ RETS Library
 *
 * librets is a C++ RETS library.  It provides support for logging in,
 * logging out, searching, and metadata retrieval.  The main interface
 * is through the librets::RetsSession class.  Here is a example
 * demonstrationg searching:
 *
 * @verbinclude "search.cpp"
 *
 * Here is an example showing how to use metadata:
 *
 * @verbinclude "metadata.cpp"
 */

#include "librets/RetsSession.h"
#include "librets/RetsException.h"
#include "librets/LogoutResponse.h"
#include "librets/SearchRequest.h"
#include "librets/SearchResultSet.h"

#include "librets/MetadataElement.h"
#include "librets/MetadataSystem.h"
#include "librets/MetadataResource.h"
#include "librets/MetadataClass.h"
#include "librets/MetadataTable.h"
#include "librets/MetadataUpdate.h"
#include "librets/MetadataUpdateType.h"
#include "librets/MetadataObject.h"
#include "librets/MetadataSearchHelp.h"
#include "librets/MetadataEditMask.h"
#include "librets/MetadataUpdateHelp.h"
#include "librets/MetadataLookup.h"
#include "librets/MetadataLookupType.h"
#include "librets/MetadataValidationLookup.h"
#include "librets/MetadataValidationLookupType.h"
#include "librets/MetadataValidationExternal.h"
#include "librets/MetadataValidationExternalType.h"
#include "librets/MetadataValidationExpression.h"
#include "librets/MetadataForeignKey.h"

#include "librets/RetsMetadata.h"

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
