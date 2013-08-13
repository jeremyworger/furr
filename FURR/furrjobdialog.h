#ifndef _FURRJOBDLG_H
#define _FURRJOBDLG_H

#include "stdafx.h"
#include "furrtypes.h"

/*
 * FURRJobDialog Class
 *
 * Represents a dialog that
 * has a job status result
 * that can be queried by the
 * calling code.
 */
class FURRJobDialog : public Dialog {

public:
  FURRJobDialog() {

    m_Status  = FAILED;
    m_pRS     = NULL;
  }
  virtual ~FURRJobDialog() {

  }

  virtual void Die(JobStatus Status) { m_Status = Status; EndDialog(0); }
  JobStatus GetStatus(void) { return m_Status; }

  void SetRecordSessionData(PRECORDSESSION pRS) { m_pRS = pRS; }
  
protected:
  JobStatus       m_Status;
  PRECORDSESSION  m_pRS;

};

#endif // !_FURRJOBDLG_H