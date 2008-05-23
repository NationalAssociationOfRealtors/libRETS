/*
 * Copyright (C) 2005-2008 National Association of REALTORS(R)
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

#ifndef LIBRETS_SQL_TO_DMQL_COMPILER_H
#define LIBRETS_SQL_TO_DMQL_COMPILER_H

#include <iosfwd>
#include "librets/sql_forward.h"
#include "librets/RetsObject.h"

namespace librets {

class SqlToDmqlCompiler : public RetsObject
{
  public:
    SqlToDmqlCompiler(SqlMetadataPtr metadata);
    
    enum QueryType {DMQL_QUERY, GET_OBJECT_QUERY, LOOKUP_QUERY,
                    LOOKUP_COLUMNS_QUERY};
    
    QueryType sqlToDmql(std::string sql);
    
    QueryType sqlToDmql(std::istream & inputStream);
    
    DmqlQueryPtr GetDmqlQuery() const;
    
    GetObjectQueryPtr GetGetObjectQuery() const;

    LookupQueryPtr GetLookupQuery() const;
    
    LookupColumnsQueryPtr GetLookupColumnsQuery() const;
    
 private:
    SqlMetadataPtr mMetadata;
    DmqlQueryPtr mDmqlQuery;
    GetObjectQueryPtr mGetObjectQuery;
    LookupQueryPtr mLookupQuery;
    LookupColumnsQueryPtr mLookupColumnsQuery;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
