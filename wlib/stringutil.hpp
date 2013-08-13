#ifndef _WLIB_STRINGUTIL_H
#define _WLIB_STRINGUTIL_H

#include <algorithm>
#include <locale>

/*
 * Function definitions
 */
void StrToUpper(TSTRING &tsModify);
void StrToLower(TSTRING &tsModify);
void StrStripChar(TSTRING &tsModify, const TCHAR cStrip);
bool StrStripChars(TSTRING &tsModify, const TCHAR *szStrip);

/*
 * Classes
 */
class SubStringInString : public std::unary_function<TCHAR, bool> {

public:
  SubStringInString(const TCHAR *szStr) : m_Str(szStr) { }
  ~SubStringInString() { }

  bool operator()(const TCHAR tcFind) {

    return (TSTRING::npos != m_Str.find(tcFind));
  }

private:
  TSTRING m_Str;

};

class CharInString : public std::unary_function<TCHAR, bool> {

public:
  CharInString(const TCHAR tcFind) : m_tcFind(tcFind) { }
  ~CharInString() { }

  bool operator()(const TCHAR tcFind) {

    return (tcFind == m_tcFind);
  }

private:
  TCHAR m_tcFind;

};

#endif // !_WLIB_STRINGUTIL_H
