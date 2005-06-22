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

#include <boost/shared_ptr.hpp>

namespace librets {

class DmqlCriterion;
typedef boost::shared_ptr<DmqlCriterion> DmqlCriterionPtr;

class JunctionCriterion;
typedef boost::shared_ptr<JunctionCriterion> JunctionCriterionPtr;

class LiteralCriterion;
typedef boost::shared_ptr<LiteralCriterion> LiteralCriterionPtr;

class EqCriterion;
typedef boost::shared_ptr<EqCriterion> EqCriterionPtr;

class GtCriterion;
typedef boost::shared_ptr<GtCriterion> GtCriterionPtr;

class LtCriterion;
typedef boost::shared_ptr<LtCriterion> LtCriterionPtr;

class OrCriterion;
typedef boost::shared_ptr<OrCriterion> OrCriterionPtr;

class AndCriterion;
typedef boost::shared_ptr<AndCriterion> AndCriterionPtr;

class NotCriterion;
typedef boost::shared_ptr<NotCriterion> NotCriterionPtr;

class DmqlQuery;
typedef boost::shared_ptr<DmqlQuery> DmqlQueryPtr;

class GetObjectQuery;
typedef boost::shared_ptr<GetObjectQuery> GetObjectQueryPtr;

class SqlToDmqlCompiler;
typedef boost::shared_ptr<SqlToDmqlCompiler> SqlToDmqlCompilerPtr;

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
