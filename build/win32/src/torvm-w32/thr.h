/* Copyright (C) 2008-2009  The Tor Project, Inc.
 * See LICENSE file for rights and terms.
 */
#ifndef __thr_h__
#define __thr_h__

#include "torvm.h"

/* XXX: these should probably be macros or inline but for now the
 * stack frames are useful for debugging.
 */
/* Critical section primitives. */
BOOL  createcs (LPCRITICAL_SECTION cs);
BOOL  destroycs (LPCRITICAL_SECTION cs);
BOOL  entercs (LPCRITICAL_SECTION cs);
BOOL  leavecs (LPCRITICAL_SECTION cs);

/* Mutex primitives. */
BOOL  createlock (LPHANDLE lockptr);
BOOL  destroylock (HANDLE lockptr);
BOOL  trylock (HANDLE lock);
BOOL  waitlock (HANDLE lock,
                DWORD  mstimout);
BOOL  unlock (HANDLE lock);

/* Semaphore signalling primitives. */
#ifndef MAXSEMCOUNT
#define MAXSEMCOUNT 32
#endif
BOOL  createsem (LPHANDLE semptr,
                 LONG     limit,
                 BOOL     startsignaled);
BOOL  destroysem (HANDLE semptr);
BOOL  trysem (HANDLE semptr);
BOOL  waitsem (HANDLE semptr,
               DWORD  mstimout);
BOOL  signalsem (HANDLE semptr);

/* Thread primitives. */
typedef DWORD (__stdcall *PFnThreadMain)(LPVOID param);
BOOL  createthr (PFnThreadMain  thrmain,
                 LPVOID         arg,
                 BOOL           suspended);
BOOL  destroythr (HANDLE thr);
BOOL  pausethr (HANDLE thr);
BOOL  resumethr (HANDLE thr);
VOID  exitthr (DWORD exitcode);
BOOL  checkthr (HANDLE thr,
                LPDWORD retval);
BOOL  waitforthr (HANDLE thr,
                  DWORD  mstimout,
                  LPDWORD retval);
BOOL  waitforallthr (const HANDLE *thrlist,
                     DWORD         count,
                     DWORD         mstimout);
BOOL  waitforanythr (const HANDLE *thrlist,
                     DWORD         count,
                     DWORD         mstimout,
                     LPHANDLE      signaledhnd);

/* IMPORTANT: the main process thread needs to initialze various thread
 * tracking structures at start to ensure the helper routines below have
 * the context needed to keep track of all threads and ids.
 */
BOOL  setupthrctx (VOID);

/* Must be called from main before exit to clean up allocated structures.
 * Provided in anticipation of memory profiling where dangling
 * allocations introduce false positives.
 */
VOID  cleanupthrctx (VOID);

/* Keep a list of threads so that we know their relative IDs and can forcibly
 * terminate them if needed.  The logging code in particular uses relative
 * thread IDs indicative of create order rather than the sometimes opaque
 * system assigned identifiers or handles.
 */
struct s_thrinfo {
  HANDLE  hnd;
  DWORD   id;
  int     num;
  struct s_thrinfo *next;
};

int thrnum (VOID);
BOOL getthrnum (HANDLE thrhnd,
                int*   num);
int numthreads (VOID);

#endif /* thr_h */
