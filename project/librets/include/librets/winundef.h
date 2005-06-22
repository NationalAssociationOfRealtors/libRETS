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

/*
 * This file does not have include guards, so it may be included more
 * than once.
 *
 * The purpose of this file is to undo macro definitions that Windows
 * header files define which conflict with method names.  Windows does
 * this to support Unicode versions of its functions.  For example, it
 * will #define GetMessage to GetMessageA or GetMessageU depending on
 * the Unicode settings.  We don't care about these functions, so we
 * can just undef the macros, but a more proper solution would be to
 * define inline functions rather than macros to handle these.  The
 * wxWdigets library contains a great example of this, specifically
 * its wx/msw/winundef.h.
 */


#ifdef GetMessage
#undef GetMessage
#endif

#ifdef GetClassName
#undef GetClassName
#endif

#ifdef GetObject
#undef GetObject
#endif
