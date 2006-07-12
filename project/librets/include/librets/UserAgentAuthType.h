/*
 * Copyright (C) 2006 National Association of REALTORS(R)
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

#ifndef LIBRETS_RETS_USER_AGENT_AUTH_TYPE_H
#define LIBRETS_RETS_USER_AGENT_AUTH_TYPE_H

namespace librets {
    
enum UserAgentAuthType
{
    /**
     * RETS 1.7 user agent authorization.
     */
    USER_AGENT_AUTH_RETS_1_7,
    
    /**
     * The Interealty variant of 1.7 user agent authorization.
     */
    USER_AGENT_AUTH_INTEREALTY,
};

}

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
