
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
