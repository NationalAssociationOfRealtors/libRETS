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
#ifndef LIBRETS_SQL_FORWARD_H
#define LIBRETS_SQL_FORWARD_H

/**
 * @file sql_forward.h
 * (Internal) Contains the Sql to Dmql Compiler protocol forward
 * declarations.
 */
/// @cond MAINTAINER

#include <boost/shared_ptr.hpp>

namespace librets {

class DmqlCriterion;
/** Smart pointer to DmqlCriterion */
typedef boost::shared_ptr<DmqlCriterion> DmqlCriterionPtr;

class JunctionCriterion;
/** Smart pointer to JunctionCriterion */
typedef boost::shared_ptr<JunctionCriterion> JunctionCriterionPtr;

class LookupCriterion;
/** Smart pointer to LookupCriterion */
typedef boost::shared_ptr<LookupCriterion> LookupCriterionPtr;

class LiteralCriterion;
/** Smart pointer to LiteralCriterion */
typedef boost::shared_ptr<LiteralCriterion> LiteralCriterionPtr;

class EqCriterion;
/** Smart pointer to EqCriterion */
typedef boost::shared_ptr<EqCriterion> EqCriterionPtr;

class GtCriterion;
/** Smart pointer to GtCriterion */
typedef boost::shared_ptr<GtCriterion> GtCriterionPtr;

class LtCriterion;
/** Smart pointer to LtCriterion */
typedef boost::shared_ptr<LtCriterion> LtCriterionPtr;

class OrCriterion;
/** Smart pointer to OnCriterion */
typedef boost::shared_ptr<OrCriterion> OrCriterionPtr;

class AndCriterion;
/** Smart pointer to AndCriterion */
typedef boost::shared_ptr<AndCriterion> AndCriterionPtr;

class NotCriterion;
/** Smart pointer to NotCriterion */
typedef boost::shared_ptr<NotCriterion> NotCriterionPtr;

class LookupOrCriterion;
/** Smart pointer to LookupOrCriterion */
typedef boost::shared_ptr<LookupOrCriterion> LookupOrCriterionPtr;

class DmqlQuery;
/** Smart pointer to DmqlQuery */
typedef boost::shared_ptr<DmqlQuery> DmqlQueryPtr;

class GetObjectQuery;
/** Smart pointer to GetObjectQuery */
typedef boost::shared_ptr<GetObjectQuery> GetObjectQueryPtr;

class LookupQuery;
/** Smart pointer to LookupQuery */
typedef boost::shared_ptr<LookupQuery> LookupQueryPtr;

class LookupColumnsQuery;
/** Smart pointer to LookupColumnsQuery */
typedef boost::shared_ptr<LookupColumnsQuery> LookupColumnsQueryPtr;

class SqlToDmqlCompiler;
/** Smart pointer to SqlToDmqlCompiler */
typedef boost::shared_ptr<SqlToDmqlCompiler> SqlToDmqlCompilerPtr;

class SqlMetadata;
/** Smart pointer to SqlMetadata */
typedef boost::shared_ptr<SqlMetadata> SqlMetadataPtr;

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
