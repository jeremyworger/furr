#ifndef _FURRTYPES_H
#define _FURRTYPES_H

#include "stdafx.h"
#include "../ImageMagick/include/Magick++.h"
#include "adjustparams.h"

using namespace Magick;

/*
 * JobStatus enum
 *
 * Contains all possible values
 * for the return value of a job
 */
enum JobStatus {

  SUCCEEDED = 0,
  FAILED,
  CANCELLED

};

/*
 * ANNOTATION Structure
 *
 * Represents data associated
 * with annotation text to be drawn
 * on a frame during a session.
 */
typedef struct tagANNOTATION {

  tagANNOTATION() {

    Reset();

  }

  tagANNOTATION &operator=(const tagANNOTATION &in) {

    bUseAnnotation  = in.bUseAnnotation;
    szText          = in.szText;
    dFontSize       = in.dFontSize;
    dTextRed        = in.dTextRed;
    dTextGreen      = in.dTextGreen;
    dTextBlue       = in.dTextBlue;
    drawX           = in.drawX;
    drawY           = in.drawY;
    drawCX          = in.drawCX;
    drawCY          = in.drawCY;

    return (*this);
  }

  bool bUseAnnotation;
  const char *szText;
  double dFontSize;
  double dTextRed;
  double dTextGreen;
  double dTextBlue;
  int drawX;
  int drawY;
  int drawCX;
  int drawCY;

  void Reset(void) {

    bUseAnnotation  = false;
    szText          = NULL;
    dFontSize       = 24.0;
    dTextRed        = 1.0;
    dTextGreen      = 1.0;
    dTextBlue       = 1.0;
    drawX           = 0;
    drawY           = 0;
    drawCX          = 0;
    drawCY          = 0;

  }

} ANNOTATION, *PANNOTATION;

/*
 * FRAME Structure
 *
 * Represents information about a frame
 * (image file on disk, or memory)
 */
typedef struct tagFRAME {

  tagFRAME() {

    tsFileName  = _T("");

  }

  TSTRING tsFileName;   // Image file on disk

} FRAME, *PFRAME;

/*
 * FRAMELIST: List of FRAME structures
 */
typedef std::list<FRAME> FRAMELIST, *PFRAMELIST;

/*
 * RECORDSESSION Structure
 *
 * Contains information required by
 * any module to display or access
 * data associated with a session.
 */
typedef struct tagRECORDSESSION {

  tagRECORDSESSION() {

    Rewind();

  }

  tagRECORDSESSION &operator=(const tagRECORDSESSION &in) {

    tsDirectory = in.tsDirectory;
    lstFrames   = in.lstFrames;
    _it         = lstFrames.begin();
    AP          = in.AP;
    Annotation  = in.Annotation;

    return (*this);

  }

  TSTRING tsDirectory;      // Directory where frames reside
  FRAMELIST lstFrames;      // List of frames in this session
  FRAMELIST::iterator _it;  // Interally-used iterator
  ADJUSTPARAMS AP;          // Image parameters
  ANNOTATION Annotation;    // Annotation optional settings

  CTSTRING &Directory(void) { return tsDirectory; }

  FRAMELIST::size_type FrameCount(void) { return lstFrames.size(); }

  void Reset(void) {

    tsDirectory.clear();
    lstFrames.clear();
    _it = lstFrames.begin();
    AP.Reset();
    Annotation.Reset();

  }

  void Rewind(void) { _it = lstFrames.begin(); }

  bool GetCurFrame(FRAME &tsOut) {

    bool r = false;

    if (0 < lstFrames.size()) {

      if (_it != lstFrames.end()) {

        tsOut = (*_it);
        r     = true;

      }

    }

    return r;
  }

  bool EnumFrames(FRAME &tsOut) {

    bool r = false;

    if (0 < lstFrames.size()) {

      if (_it != lstFrames.end()) {

        tsOut = (*_it);
        r     = true;
        _it++;

      } else {

        _it = lstFrames.begin();
        
      }

    }

    return r;
  }

} RECORDSESSION, *PRECORDSESSION;

#endif // !_FURRTYPES_H
