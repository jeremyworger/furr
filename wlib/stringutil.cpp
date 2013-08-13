#include "stdafx.h"
#include "stringutil.hpp"

void StrToUpper(TSTRING &tsModify) {

  TSTRING::iterator it = tsModify.begin();

  for (; it != tsModify.end(); it++) {

    (*it) = toupper((*it));

  }

}

void StrToLower(TSTRING &tsModify) {

  TSTRING::iterator it = tsModify.begin();

  for (; it != tsModify.end(); it++) {

    (*it) = tolower((*it));

  }

}

void StrStripChar(TSTRING &tsModify, const TCHAR cStrip) {

  tsModify.erase(std::remove_if(tsModify.begin(), tsModify.end(),
                 CharInString(cStrip)), tsModify.end());
}

bool StrStripChars(TSTRING &tsModify, const TCHAR *szStrip) {

  bool r = false;

  if (NULL != szStrip) {

    tsModify.erase(std::remove_if(tsModify.begin(), tsModify.end(),
                   SubStringInString(szStrip)), tsModify.end());
  
    r = true;

  }  

  return r;
}
