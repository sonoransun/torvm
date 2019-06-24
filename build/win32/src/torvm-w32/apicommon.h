/* Copyright (C) 2008-2009  The Tor Project, Inc.
 * See LICENSE file for rights and terms.
 */
#ifndef __apicommon_h__
#define __apicommon_h__

/* enable certain parts of the win32 API for process / system functions
 * default to win2k.  2K is minimum supported due to Crypto API dependencies
 * and other linkage.  other possible versions to enforce:
 *   0x0501 = XP / Server2003
 *   0x0502 = XP SP2 / Server2003 SP1
 *   0x0600 = Vista / Server2008
 */
#define _WIN32_WINNT 0x0500

/* Prevent inclusion of the old Winsock.h 1.1 headers
 */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <iphlpapi.h>

#include <security.h>
#include <tchar.h>
#include <winreg.h>
#include <winioctl.h>
#include <winerror.h>
#include <wincrypt.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/* misc win32 api utility functions
 */
#define CMDMAX         4096
#define DEFAULT_WINDIR "C:\\WINDOWS"
#define TOR_VM_BASE    "Tor_VM"
#define W_TOR_VM_BASE  L"Tor_VM"
#define TOR_VM_BIN     "bin"
#define TOR_VM_LIB     "lib"
#define TOR_VM_STATE   "state"
BOOL buildpath (const TCHAR *dirname,
                TCHAR **fullpath);
#define PATH_FQ        1
#define PATH_RELATIVE  2
#define PATH_MSYS      3
#define VMDIR_BASE     1
#define VMDIR_BIN      2
#define VMDIR_LIB      3
#define VMDIR_STATE    4
BOOL buildfpath (DWORD   pathtype,
                 DWORD   subdirtype,
                 LPTSTR  wdpath,
                 LPTSTR  append,
                 LPTSTR *fpath);
#define SYSDIR_WINROOT     1
#define SYSDIR_PROFILE     2
#define SYSDIR_ALLPROFILE  3
#define SYSDIR_PROGRAMS    4
#define SYSDIR_LCLDATA     5
#define SYSDIR_LCLPROGRAMS 6
BOOL buildsyspath (DWORD   syspathtype,
                   LPTSTR  append,
                   LPTSTR *fpath);
BOOL getmypath (TCHAR **path);
BOOL exists (LPTSTR path);
BOOL copyfile (LPTSTR srcpath,
               LPTSTR destpath);
BOOL getprocwd (TCHAR **cwd);
BOOL setprocwd (const TCHAR *cwd);

/* localize a handle resource to the current process (no inherit)
 * types of handles supported:
 *   Access token
 *   Change notification
 *   Communications device
 *   Console input
 *   Console screen buffer
 *   Desktop
 *   Event
 *   File
 *   File mapping
 *   Job
 *   Mailslot
 *   Mutex
 *   Pipe
 *   Process
 *   Registry key
 *   Semaphore
 *   Thread
 *   Time
 *   Window station
 */
BOOL localhnd (HANDLE  *hnd);

/* duplicating files in other processes does not close original */
BOOL proclocalhnd (HANDLE srcproc,
                   HANDLE dstproc,
                   HANDLE *hnd);

/* get the current Windows OS version.  this is needed for things like the network
 * configuration export and some API calls.
 */
#define OS_UNKNOWN     0
#define OS_SERVER2008  5
#define OS_VISTA       4
#define OS_SERVER2003  3
#define OS_XP          2
#define OS_2000        1

int getosversion (void);
int getosbits (void);

typedef struct s_CmdInfo {
  BOOL   isrunning;
  DWORD  status;
  HANDLE hnd;
  HANDLE stdin_rd;
  HANDLE stdin_wr;
  HANDLE stdout_rd;
  HANDLE stdout_wr;
} CmdInfo;

BOOL getcompguid (TCHAR **guid);
void bgstartupinfo (STARTUPINFO *si);
/*
BOOL launchcommand(LPSTR cmd,
                   LPSTR dir,
                   CmdInfo **info);
BOOL checkcommand(CmdInfo *info);
BOOL killcommand(CmdInfo *info);
*/
BOOL runcommand(LPSTR cmd,
                LPSTR dir);

BOOL getmacaddr(const char *  devguid,
                char **       mac);
BOOL isconnected(const char *  devguid);

BOOL tryconnect(const char * addr,
                DWORD port);

BOOL rmdirtree(LPSTR path);

BOOL entropy(DWORD   len,
             BYTE ** rndbuf);

#endif /* apicommon_h */
