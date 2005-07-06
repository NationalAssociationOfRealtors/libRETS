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

#include "librets/forward.h"

#include "librets/RetsSession.h"
#include "librets/RetsException.h"
#include "librets/CapabilityUrls.h"
#include "librets/LoginResponse.h"
#include "librets/LogoutResponse.h"
#include "librets/SearchRequest.h"
#include "librets/SearchResultSet.h"
#include "librets/GetObjectRequest.h"
#include "librets/GetObjectResponse.h"
#include "librets/ObjectDescriptor.h"

#include "librets/RetsMetadata.h"
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

#include "librets/RetsXmlParser.h"
#include "librets/RetsXmlAttribute.h"
#include "librets/RetsXmlEvent.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"
#include "librets/RetsXmlTextEvent.h"

#include "librets/RetsHttpClient.h"
#include "librets/RetsHttpRequest.h"
#include "librets/RetsHttpResponse.h"
#include "librets/RetsHttpLogger.h"
#include "librets/StreamHttpLogger.h"

#include "librets/SqlToDmqlCompiler.h"
#include "librets/DmqlQuery.h"
#include "librets/DmqlCriterion.h"

#include "librets/util.h"

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
