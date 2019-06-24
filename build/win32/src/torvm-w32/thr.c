#include "torvm.h"

/* Some statics to keep track of things...
 * XXX: note that this is inherently unaware of a thread handle
 * allocated by an external process with privs in the Tor VM process.
 * Inter-process threading and locking is explicitly not provided.
 */
static LPCRITICAL_SECTION  s_thridx_cs = NULL;
static DWORD s_thrcount = 0;
static DWORD s_currthrnum = 0;
static struct s_thrinfo *  s_thrlist = NULL;

BOOL  createcs (LPCRITICAL_SECTION cs)
{
  /* The high bit is set to pre-allocate any necessary resources so that
   * a low memory condition does introduce an exception leading to ugly
   * failure recovery...
   */
  if (!InitializeCriticalSectionAndSpinCount(cs, 0x80000400))
    return FALSE;
  return TRUE;
}

BOOL  destroycs (LPCRITICAL_SECTION cs)
{
  if (!cs)
    return FALSE;
  DeleteCriticalSection(cs);
  return TRUE;
}

BOOL  entercs (LPCRITICAL_SECTION cs)
{
  if (!cs)
    return FALSE;
  EnterCriticalSection(cs);
  return TRUE;
}

BOOL  leavecs (LPCRITICAL_SECTION cs)
{
  if (!cs)
    return FALSE;
  LeaveCriticalSection(cs);
  return TRUE;
}

BOOL  createlock (LPHANDLE lockptr)
{
  *lockptr = CreateMutex(0, FALSE, 0);
  return TRUE;
}

BOOL  destroylock (HANDLE lockptr)
{
  return TRUE;
}

BOOL  trylock (HANDLE lock)
{
  return TRUE;
}

BOOL  waitlock (HANDLE lock,
                DWORD  mstimout)
{
  return TRUE;
}

BOOL  unlock (HANDLE lock)
{
  return TRUE;
}


/* Semaphore signalling primitives. */
BOOL  createsem (LPHANDLE semptr,
                 LONG     limit,
                 BOOL     startsignaled)
{
  DWORD icount = 0;
  if (limit > MAXSEMCOUNT) limit = MAXSEMCOUNT;
  if (startsignaled == TRUE) icount = limit;
  *semptr = CreateSemaphore(0,
                            icount,
                            limit,
                            0);
  return TRUE;
}

BOOL  destroysem (HANDLE semptr)
{
  return TRUE;
}
 
BOOL  trysem (HANDLE semptr)
{
  return TRUE;
}
 
BOOL  waitsem (HANDLE semptr,
               DWORD  mstimout)
{
  return TRUE;
}

BOOL  signalsem (HANDLE semptr)
{
  return TRUE;
}

BOOL  createthr (PFnThreadMain  thrmain,
                 LPVOID         arg,
                 BOOL           suspended)
{
  BOOL retval = FALSE;
  LPTHREAD_START_ROUTINE f = (LPTHREAD_START_ROUTINE) thrmain;
  DWORD tid;
  DWORD cflags = CREATE_SUSPENDED;
  HANDLE newthr;
  struct s_thrinfo * thrinfo = NULL;

  entercs(s_thridx_cs);
  newthr = CreateThread(NULL,
                        0,
                        f,
                        arg,
                        cflags,
                        &tid);
  if (!newthr)
    goto finish;

  thrinfo = malloc(sizeof(struct s_thrinfo));
  thrinfo->next = s_thrlist;
  thrinfo->hnd = newthr;
  thrinfo->id = tid;
  thrinfo->num = s_currthrnum++;
  s_thrcount++;
  s_thrlist = thrinfo;
  retval = TRUE;

 finish:
  leavecs(s_thridx_cs);
  if (retval && !suspended)
    ResumeThread(newthr);

  return(retval);
}

BOOL  destroythr (HANDLE thr)
{
  BOOL retval = FALSE;
  struct s_thrinfo * cinfo = NULL;
  struct s_thrinfo * delthrinfo = NULL;

  entercs(s_thridx_cs);
    
  leavecs(s_thridx_cs);

  return(retval);
}

BOOL  pausethr (HANDLE thr)
{
  return TRUE;
}

BOOL  resumethr (HANDLE thr)
{
  return TRUE;
}

VOID  exitthr (DWORD exitcode)
{
  return;
}
 
BOOL  checkthr (HANDLE thr,
                LPDWORD retval)
{
  return TRUE;
}

BOOL  waitforthr (HANDLE thr,
                  DWORD  mstimout,
                  LPDWORD retval)
{
  return TRUE;
}

BOOL  waitforallthr (const HANDLE *thrlist,
                     DWORD         count,
                     DWORD         mstimout)
{
  return TRUE;
}

BOOL  waitforanythr (const HANDLE *thrlist,
                     DWORD         count,
                     DWORD         mstimout,
                     LPHANDLE      signaledhnd)
{
  return TRUE;
}

BOOL  setupthrctx (VOID)
{
  s_thridx_cs = malloc(sizeof(CRITICAL_SECTION));
  if (!s_thridx_cs)
    return FALSE;
  createcs(s_thridx_cs);
  entercs(s_thridx_cs);
  s_thrlist = malloc(sizeof(struct s_thrinfo));
  s_thrlist->next = NULL;
  s_thrlist->hnd = GetCurrentThread();
  s_thrlist->id = GetCurrentThreadId();
  s_thrlist->num = s_currthrnum++;
  s_thrcount++;
  leavecs(s_thridx_cs);
  return TRUE;
}

VOID  cleanupthrctx (VOID)
{
  return;
}

int thrnum (VOID)
{
  int retval = -1;
  struct s_thrinfo * thrinfo = NULL;
  entercs(s_thridx_cs);
  thrinfo = s_thrlist;
  while (thrinfo && (retval < 0)) {
    if (thrinfo->id == GetCurrentThreadId())
      retval = thrinfo->num;
    thrinfo = thrinfo->next;
  }
  leavecs(s_thridx_cs);
  return (retval);
}

BOOL getthrnum (HANDLE thrhnd,
                int*   num)
{
  *num = -1;
  struct s_thrinfo * thrinfo = NULL;
  entercs(s_thridx_cs);
  thrinfo = s_thrlist;
  while (thrinfo && (*num < 0)) {
    if (thrinfo->hnd == thrhnd)
      *num = thrinfo->num;
    thrinfo = thrinfo->next;
  }
  leavecs(s_thridx_cs);
  if (*num < 0)
    return FALSE;
  return TRUE;
}

int numthreads (VOID)
{
  int retval = 0;
  entercs(s_thridx_cs);
  retval = s_thrcount;
  leavecs(s_thridx_cs);
  return (retval);
}

