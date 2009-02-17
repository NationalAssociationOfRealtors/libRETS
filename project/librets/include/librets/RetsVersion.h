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

#ifndef LIBRETS_RETS_VERSION_H
#define LIBRETS_RETS_VERSION_H
/** 
 * @file RetsVersion.h
 * Contains the RetsVersion enum declaration.
 */

namespace librets {

/**
 * RetsVersion contains the definitions of types the various protocol
 * versions of RETS servers.
 */
enum RetsVersion
{
    /**
     * This is a RETS 1.0 server.
     */
    RETS_1_0,
    /**
     * This is a RETS 1.5 server.
     */
    RETS_1_5,
    /**
     * This is a RETS 1.7 server.
     */
    RETS_1_7,
    /**
     * This is a RETS 1.7.2 server.
     */
    RETS_1_7_2
};

}

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
