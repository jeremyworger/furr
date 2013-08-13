/*
  Methods to Lock and Unlock Semaphores.
*/
#ifndef _MAGICK_SEMAPHORE_H
#define _MAGICK_SEMAPHORE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef struct SemaphoreInfo
  SemaphoreInfo;

extern MagickExport SemaphoreInfo
  *AllocateSemaphoreInfo(void);

extern MagickExport MagickBooleanType
  LockSemaphoreInfo(SemaphoreInfo *),
  UnlockSemaphoreInfo(SemaphoreInfo *);

extern MagickExport void
  AcquireSemaphoreInfo(SemaphoreInfo **),
  DestroySemaphore(void),
  DestroySemaphoreInfo(SemaphoreInfo **),
  InitializeSemaphore(void),
  RelinquishSemaphoreInfo(SemaphoreInfo **);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
