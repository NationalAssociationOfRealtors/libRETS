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

class SqlToDmqlCompiler;
typedef boost::shared_ptr<SqlToDmqlCompiler> SqlToDmqlCompilerPtr;

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
