#ifndef _WLIB_STDAFX_H
#define _WLIB_STDAFX_H

#include <Windows.h>
#include <tchar.h>
#include <commctrl.h>
#include <string>
#include <list>
#include <stdarg.h>
#include <shlwapi.h>

/*
 * ANSISTRING
 */
typedef std::basic_string<char> ANSISTRING, *PANSISTRING;
typedef const ANSISTRING CANSISTRING, *PCANSISTRING;
typedef ANSISTRING::iterator ANSISTRINGIT, *PANSISTRINGIT;
typedef ANSISTRING::const_iterator CANSISTRINGIT, *PCANSISTRINGIT;

/*
 * TSTRING
 */
typedef std::basic_string<TCHAR> TSTRING, *PTSTRING;
typedef const TSTRING CTSTRING, *PCTSTRING;
typedef TSTRING::iterator TSTRINGIT, *PTSTRINGIT;
typedef const TSTRINGIT CTSTRINGIT, *PCTSTRINGIT;

/*
 * TSTRINGLIST
 */
typedef std::list<TSTRING> TSTRINGLIST, *PTSTRINGLIST;
typedef TSTRINGLIST::iterator TSTRINGLISTIT, *PTSTRINGLISTIT;

#endif // _WLIB_STDAFX_H
