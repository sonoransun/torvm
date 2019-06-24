/* Copyright (C) 2008-2009  The Tor Project, Inc.
 * See LICENSE file for rights and terms.
 */

#include "apicommon.h"

/* Depending on _WIN32_WINNT version and mingw32 api we may
 * have all of the socket structures defined. Needed by default.
 */
#ifndef __HAVE_IN_ADDR
typedef struct _in_addr {
  union {
    struct {
      unsigned char s_b1,
                    s_b2,
                    s_b3,
                    s_b4;
    } S_un_b;
    struct {
      unsigned short s_w1,
                     s_w2;
    } S_un_w;
    unsigned long S_addr;
  } S_un;
} in_addr;
#endif
#ifndef __HAVE_SOCKADDR_IN
typedef struct _sockaddr_in{
   short sin_family;
   unsigned short sin_port;
   struct in_addr sin_addr;
   char sin_zero[8];
} sockaddr_in;
#endif

/* jump hoops to read ethernet adapter MAC address.
 */
#define _NDIS_CONTROL_CODE(request,method) \
	CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, request, method, FILE_ANY_ACCESS)
#define IOCTL_NDIS_QUERY_GLOBAL_STATS   _NDIS_CONTROL_CODE( 0, METHOD_OUT_DIRECT )

/* these are not yet used, but we may need them to interact with network devices
 * directly to enable/disable, change MAC, etc.
 */
#define IOCTL_NDIS_QUERY_ALL_STATS      _NDIS_CONTROL_CODE( 1, METHOD_OUT_DIRECT )
#define IOCTL_NDIS_ADD_DEVICE           _NDIS_CONTROL_CODE( 2, METHOD_BUFFERED )
#define IOCTL_NDIS_DELETE_DEVICE        _NDIS_CONTROL_CODE( 3, METHOD_BUFFERED )
#define IOCTL_NDIS_TRANSLATE_NAME       _NDIS_CONTROL_CODE( 4, METHOD_BUFFERED )
#define IOCTL_NDIS_ADD_TDI_DEVICE       _NDIS_CONTROL_CODE( 5, METHOD_BUFFERED )
#define IOCTL_NDIS_NOTIFY_PROTOCOL      _NDIS_CONTROL_CODE( 6, METHOD_BUFFERED )
#define IOCTL_NDIS_GET_LOG_DATA         _NDIS_CONTROL_CODE( 7, METHOD_OUT_DIRECT )

/* OID's we need to query */
#define OID_802_3_PERMANENT_ADDRESS             0x01010101
#define OID_802_3_CURRENT_ADDRESS               0x01010102
#define OID_GEN_MEDIA_CONNECT_STATUS            0x00010114
/* probably will never need these, but just in case ... */
#define OID_GEN_MEDIA_IN_USE                    0x00010104
#define OID_WAN_PERMANENT_ADDRESS               0x04010101
#define OID_WAN_CURRENT_ADDRESS                 0x04010102
#define OID_WW_GEN_PERMANENT_ADDRESS            0x0901010B
#define OID_WW_GEN_CURRENT_ADDRESS              0x0901010C

BOOL buildsyspath (DWORD  syspathtype,
                   LPTSTR append,
                   LPTSTR *fpath)
{
  DWORD   retval;
  DWORD   errnum;
  LPTSTR  defval = NULL;
  LPTSTR  envvar;
  LPTSTR  dsep = "\\";
  *fpath = malloc(CMDMAX * sizeof(TCHAR));
  if(*fpath == NULL) {
    lerror ("buildsyspath: out of memory.");
    free(envvar);
    return FALSE;
  }
  if (syspathtype == SYSDIR_WINROOT) {
    envvar = getenv("SYSTEMROOT");
    defval = DEFAULT_WINDIR;
  }
  else if (syspathtype == SYSDIR_ALLPROFILE)
    envvar = getenv("ALLUSERSPROFILE");
  else if (syspathtype == SYSDIR_PROFILE)
    envvar = getenv("USERPROFILE");
  else if (syspathtype == SYSDIR_PROGRAMS)
    envvar = getenv("PROGRAMFILES");
  else if (syspathtype == SYSDIR_LCLDATA)
    envvar = getenv("USERPROFILE");
  else if (syspathtype == SYSDIR_LCLPROGRAMS)
    envvar = getenv("USERPROFILE");
  if(!envvar) {
    if (defval) {
      strncpy(*fpath, defval, (CMDMAX -1));
      return TRUE;
    }
    free(*fpath);
    *fpath = 0;
    return FALSE;
  }
  if ( (syspathtype == SYSDIR_LCLPROGRAMS) || (syspathtype == SYSDIR_LCLDATA) ) {
    LPTSTR lclpost = 0;
    if (syspathtype == SYSDIR_LCLPROGRAMS)
      lclpost = "Programs";
    /* local appdata and programs is built against the user profile root */
    snprintf (*fpath, (CMDMAX -1),
              "%s%s%s%s%s%s%s",
              envvar,
              dsep,
              "Local Settings\\Application Data",
              lclpost ? dsep : "",
              lclpost ? lclpost : "",
              append ? dsep : "",
              append ? append : "");
  }
  else {
    snprintf (*fpath, (CMDMAX -1),
              "%s%s%s",
              envvar,
              append ? dsep : "",
              append ? append : "");
  }
  ldebug ("Returning system path %s for path type %d and append %s", *fpath, syspathtype, append ? append : "");
  return TRUE;
}

BOOL buildfpath (DWORD   pathtype,
                 DWORD   subdirtype,
                 LPTSTR  wdpath,
                 LPTSTR  append,
                 LPTSTR *fpath)
{
  LPTSTR basepath;
  DWORD  buflen;
  *fpath = NULL;
  LPTSTR dsep = "\\";
  if (pathtype == PATH_RELATIVE) {
    if (!wdpath) {
      basepath = strdup(".");
    }
    else {
      /* TODO: for now, we check if we're in one of the bin/lib/state subdirs
       * and adjust accordingly.  what we really need to do is is build a full
       * relative path based on cwd for situations when we might be executing
       * in a location other than the usual subdirs above.
       */
      if ( (strstr(wdpath, "\\" TOR_VM_BIN)) ||
           (strstr(wdpath, "\\" TOR_VM_LIB)) || 
           (strstr(wdpath, "\\" TOR_VM_STATE))   ) {
	basepath = (pathtype == PATH_MSYS) ? strdup("../") : strdup("..\\");
      }
    }
  }
  else {
    if (!getmypath(&basepath)) {
      lerror ("Unable to get current process working directory.");
      /* TODO: what fallbacks should be used? check common locations? */
      return FALSE;
    }
    if (pathtype == PATH_MSYS) {
      /* TODO: split drive and path, then sub dir separator */
      dsep = "/";
    }
    /* truncate off our program name from the basepath */
    if (strlen(basepath) > 1) {
      LPTSTR cp = basepath + strlen(basepath) - 1;
      while (cp > basepath && *cp) {
        if (*cp == '\\')
	  *cp = 0;
	else
	  cp--;
      }
    }
  }
  buflen = strlen(basepath) + 32; /* leave plenty of room for subdir */
  if (append)
    buflen += strlen(append);
  *fpath = malloc(buflen);
  **fpath = 0;
  if (subdirtype == VMDIR_BASE) {
    snprintf (*fpath, buflen-1,
              "%s%s%s",
	      basepath,
	      append ? dsep : "",
	      append ? append : "");
  }
  else {
    LPTSTR csd = "";
    if (subdirtype == VMDIR_BIN)
      csd = TOR_VM_BIN;
    else if (subdirtype == VMDIR_LIB)
      csd = TOR_VM_LIB;
    else if (subdirtype == VMDIR_STATE)
      csd = TOR_VM_STATE;

    snprintf (*fpath, buflen-1,
              "%s%s%s%s%s",
	      basepath,
	      dsep,
	      csd,
	      append ? dsep : "",
	      append ? append : "");
  }
  ldebug ("Returning build file path %s for path type %d subdir type %d working path %s and append %s", *fpath, pathtype, subdirtype, wdpath ? wdpath : "", append ? append : "");

  free (basepath);
  return TRUE;
}

BOOL getmypath (TCHAR **path)
{
  CHAR  mypath[MAX_PATH];
  memset (mypath, 0, sizeof(mypath));
  if (! GetModuleFileNameA(NULL,
                           mypath,
                           sizeof(mypath)-1)) {
    lerror ("Unable to obtain current program path.");
    return FALSE;
  }
  *path = strdup(mypath);
  return TRUE;
}

BOOL exists (LPTSTR path)
{
  HANDLE  hnd;
  hnd = CreateFile (path,
                    GENERIC_READ,
                    0,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);
  if (hnd == INVALID_HANDLE_VALUE) {
    return FALSE;
  } 
  CloseHandle(hnd);
  return TRUE;
} 

BOOL copyfile (LPTSTR srcpath,
               LPTSTR destpath)
{ 
  HANDLE src, dest;
  DWORD buffsz = CMDMAX;   
  DWORD len, written;
  LPTSTR buff;
  src = CreateFile (srcpath,
                    GENERIC_READ,
                    0,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);
  if (src == INVALID_HANDLE_VALUE) {
    return FALSE;
  }                 
  DeleteFile (destpath);
  dest = CreateFile (destpath,
                     GENERIC_WRITE,
                     0,
                     NULL,
                     CREATE_NEW,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL);
  if (dest == INVALID_HANDLE_VALUE) {
    return FALSE;
  }
  buff = malloc(buffsz);
  if (!buff)
    return FALSE;
  while (ReadFile(src, buff, buffsz, &len, NULL) && (len > 0))
    WriteFile(dest, buff, len, &written, NULL);
  free (buff);
  CloseHandle (src);
  CloseHandle (dest);

  return TRUE;
}

void bgstartupinfo (STARTUPINFO *si)
{
  si->dwXCountChars = 48;
  si->dwYCountChars = 5;
  si->lpTitle = "Tor VM Win32";
  si->dwFillAttribute = BACKGROUND_BLUE | BACKGROUND_INTENSITY; 
  si->wShowWindow = SW_HIDE;
  si->dwFlags |= STARTF_USECOUNTCHARS | STARTF_USEFILLATTRIBUTE | STARTF_USESHOWWINDOW;
  return;
}

BOOL runcommand(LPSTR cmd,
                LPSTR dir)
{ 
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  SECURITY_ATTRIBUTES sattr;
  HANDLE stdin_rd;
  HANDLE stdin_wr;
  HANDLE stdout_rd;
  HANDLE stdout_wr;
  DWORD exitcode;
  /* Make sure interface configuration and other tasks operate quickly. */
  DWORD opts = CREATE_NEW_PROCESS_GROUP | HIGH_PRIORITY_CLASS;
  DWORD bufsz, numread;
  CHAR * buff = NULL;
   
  ZeroMemory( &pi, sizeof(pi) );
  ZeroMemory( &si, sizeof(si) );
  ZeroMemory( &sattr, sizeof(sattr) );
  si.cb = sizeof(si);
  sattr.nLength = sizeof(SECURITY_ATTRIBUTES);
  sattr.bInheritHandle = TRUE;
  sattr.lpSecurityDescriptor = NULL;

  CreatePipe(&stdout_rd, &stdout_wr, &sattr, 0);
  SetHandleInformation(stdout_rd, HANDLE_FLAG_INHERIT, 0);
  CreatePipe(&stdin_rd, &stdin_wr, &sattr, 0);
  SetHandleInformation(stdin_wr, HANDLE_FLAG_INHERIT, 0);

  si.hStdError = stdout_wr;
  si.hStdOutput = stdout_wr;
  si.hStdInput = stdin_rd;
  si.dwFlags |= STARTF_USESTDHANDLES; 
         
  if( !CreateProcess(NULL,
                     cmd,
                     NULL,   // process handle no inherit
                     NULL,   // thread handle no inherit
                     TRUE,
                     opts,
                     NULL,   // environment block
                     dir,
                     &si,
                     &pi) ) {
    lerror ("Failed to launch process.  Error code: %d", GetLastError());
    return FALSE;
  }
  ldebug ("runcommand started: %s", cmd);

  CloseHandle(stdout_wr);
  CloseHandle(stdin_rd);
  CloseHandle(stdin_wr);

  bufsz = 512; /* Write to log in small chunks. */
  buff = malloc(bufsz);
  while ( GetExitCodeProcess(pi.hProcess, &exitcode) && (exitcode == STILL_ACTIVE) ) {
    while (ReadFile(stdout_rd, buff, bufsz-1, &numread, NULL) && (numread > 0)) {
      buff[numread] = 0;
      ldebug ("runcommand output: %s", buff);
    }
    Sleep (500);
  }
  ldebug ("runcommand process exited with status: %d", exitcode);
  free(buff);
  CloseHandle(stdout_rd);
  CloseHandle(pi.hThread); 
  CloseHandle(pi.hProcess);
  
  return TRUE;
}

BOOL localhnd (HANDLE  *hnd)
{
  HANDLE  orighnd = *hnd;
  /* dupe handle for no inherit */
  if (! DuplicateHandle(GetCurrentProcess(),
                        orighnd,
                        GetCurrentProcess(),
                        hnd,
                        0,
                        FALSE, /* no inherit */
                        DUPLICATE_SAME_ACCESS)) {
    lerror ("Unable to duplicate handle.  Error code: %d", GetLastError());
    *hnd = orighnd;
    return FALSE;
  }
  /* now that we know the dupe was successful, close the original handle. */
  CloseHandle (orighnd);
  return TRUE;
}

BOOL proclocalhnd (HANDLE srcproc,
                   HANDLE dstproc,
                   HANDLE *hnd)
{
  HANDLE  orighnd = *hnd;
  if (! DuplicateHandle(srcproc,
                        orighnd,
                        dstproc,
                        hnd,
                        0,
                        FALSE, /* no inherit */
                        DUPLICATE_SAME_ACCESS)) {
    lerror ("Unable to duplicate handle.  Error code: %d", GetLastError());
    *hnd = orighnd;
    return FALSE;
  }
  return TRUE;
}

BOOL getcompguid (TCHAR **guid)
{
/* MRP_TEMP this needs dynamic linkage */
  return FALSE;
  static const int  alen = 64 * sizeof(TCHAR);
  *guid = malloc(alen);
  if (! *guid)
    fatal ("Allocation failure in: %s line no: %s with sz: %d", __FILE__ , __LINE__ , alen);
#if 0
  if (! GetComputerObjectName(NameUniqueId,
                              *guid,
                              alen)) {
    lerror ("Unable to obtain computer unique id name.  Error code: %d", GetLastError());
    free (*guid);
    *guid = NULL;
    return FALSE;
  }
#endif
  return TRUE;
}

int getosversion (void) {
  static int osver = -1;

  /* used cached version info if version has been checked already */
  if (osver >= 0)
    return osver;

  osver = OS_UNKNOWN;
  OSVERSIONINFO info;
  ZeroMemory(&info, sizeof(OSVERSIONINFO));
  info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&info);
  if (info.dwMajorVersion == 5) {
    if (info.dwMinorVersion == 0) {
      ldebug ("Operating system version is Windows 2000");
      osver = OS_2000;
    }
    else if (info.dwMinorVersion == 1) {
      ldebug ("Operating system version is Windows XP");
      osver = OS_XP;
    }
    else if (info.dwMinorVersion == 2) {
      ldebug ("Operating system version is Windows Server 2003");
      osver = OS_SERVER2003;
    }
  }
  else if (info.dwMajorVersion == 6) {
    OSVERSIONINFOEXA exinfo;
    ZeroMemory(&exinfo, sizeof(OSVERSIONINFOEXA));
    exinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
    GetVersionExA(&exinfo);
    if (exinfo.wProductType != VER_NT_WORKSTATION) {
      ldebug ("Operating system version is Windows Vista");
      osver = OS_VISTA;
    }
    else {
      ldebug ("Operating system version is Windows Server 2008");
      osver = OS_SERVER2008;
    }
  }
  return osver;
}

int getosbits (void) {
  static int  osbits = -1;

  if (osbits >= 0)
    return osbits;

  osbits = 0;
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  if (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
    ldebug ("Operating system is running on 64bit architecture.");
    osbits = 64;
  }
  else if (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
    ldebug ("Operating system is running on 32bit architecture.");
    osbits = 32;
  }
  else {
    ldebug ("Operating system is running on UNKNOWN architecture.");
  }
  return osbits;
}

BOOL getmacaddr(const char *  devguid,
                char **       mac)
{
  char *  devfstr = NULL;
  unsigned char  macbuf[6];
  BOOL   status;
  HANDLE devfd;
  DWORD retsz, oidcode;
  BOOL  retval = FALSE;

  *mac = NULL;
  devfstr = malloc(1024);
  snprintf (devfstr, 1023, "\\\\.\\%s", devguid);
  devfd = CreateFile(devfstr,
                     0,
                     FILE_SHARE_READ | FILE_SHARE_WRITE, 
                     NULL,
                     OPEN_EXISTING,
                     0,
                     NULL);
  if (devfd == INVALID_HANDLE_VALUE)
  {
    /* this is a normal condition for non-ethernet devices */
    ldebug ("Unable to open net device handle for path: %s", devfstr);
    goto cleanup;
  }

#define MAXMAC 24
  oidcode = OID_802_3_CURRENT_ADDRESS;
  status = DeviceIoControl(devfd,
                           IOCTL_NDIS_QUERY_GLOBAL_STATS,
                           &oidcode, sizeof(oidcode),
                           macbuf, sizeof(macbuf),
                           &retsz,
                           (LPOVERLAPPED) NULL);
  if (retsz == sizeof(macbuf)) {
    *mac = malloc(MAXMAC);
    memset(*mac, 0, MAXMAC);
    snprintf(*mac, MAXMAC-1,
             "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
             macbuf[0], macbuf[1], macbuf[2], macbuf[3], macbuf[4], macbuf[5]);
    retval = TRUE;
  }
  else {
    retval = FALSE;
  }

 cleanup:
  if (devfd != INVALID_HANDLE_VALUE)
    CloseHandle(devfd);
  free(devfstr);

  return retval;
}

BOOL isconnected(const char *  devguid)
{
  char *  devfstr = NULL;
  BOOL   status;
  HANDLE devfd;
  DWORD retsz, oidcode, intfStatus;
  BOOL  retval = FALSE;

  devfstr = malloc(1024);
  snprintf (devfstr, 1023, "\\\\.\\%s", devguid);
  devfd = CreateFile(devfstr,
                     0,
                     FILE_SHARE_READ | FILE_SHARE_WRITE, 
                     NULL,
                     OPEN_EXISTING,
                     0,
                     NULL);
  if (devfd == INVALID_HANDLE_VALUE)
  {
    linfo ("Unable to open net device handle for path: %s", devfstr);
    goto cleanup;
  }

  oidcode = OID_GEN_MEDIA_CONNECT_STATUS;
  status = DeviceIoControl(devfd,
                           IOCTL_NDIS_QUERY_GLOBAL_STATS,
                           &oidcode, sizeof(oidcode),
                           &intfStatus, sizeof(intfStatus),
                           &retsz,
                           (LPOVERLAPPED) NULL);
  if (status) {
    ldebug ("Received media connect status %d for device %s.", intfStatus, devguid);
    retval = (intfStatus == 0) ? TRUE : FALSE;
  }
  else {
    retval = FALSE;
  }

 cleanup:
  if (devfd != INVALID_HANDLE_VALUE)
    CloseHandle(devfd);
  free(devfstr);

  return retval;
}

BOOL tryconnect(const char * addr,
                DWORD port)
{
  WSADATA wsadata;
  SOCKET csocket;
  int result = WSAStartup(MAKEWORD(2,2), &wsadata);
  csocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (csocket == INVALID_SOCKET) {
    ldebug("Error at socket(): %ld\n", WSAGetLastError());
    WSACleanup();
    return FALSE;
  }
  sockaddr_in dest;
  dest.sin_family = AF_INET;
  dest.sin_addr.s_addr = inet_addr(addr);
  dest.sin_port = htons(port);
  if (connect(csocket,
              (SOCKADDR*)&dest,
              sizeof(dest)) == SOCKET_ERROR) {
    WSACleanup();
    return FALSE;
  }
  closesocket(csocket);
  WSACleanup();
  return TRUE;
}

BOOL rmdirtree(LPSTR path)
{
  LPSTR cmd = NULL;
  cmd = malloc(CMDMAX);
  ldebug("Removing directory tree at path: %s", path);
  snprintf(cmd, CMDMAX -1, "rmdir.exe /S /Q \"%s\"", path);
  runcommand(cmd,NULL);
  free(cmd);
  return TRUE;
}

/* NOTE: because of possibly insecure/exposed PRNG state on some win32 hosts
 *       we must read past the first 128Kbytes of generator output before
 *       using any entropy from the pool.
 *       http://eprint.iacr.org/2007/419 
 */
BOOL entropy(DWORD   len,
             BYTE ** rndbuf)
{
  HCRYPTPROV provhnd;
  int retval, i;
  BYTE *nullbuf = NULL;
  DWORD nblen = 1024;
  *rndbuf = NULL;
  retval = CryptAcquireContext(&provhnd, NULL, NULL, PROV_RSA_FULL, 0);
  if (retval == 0) {
    lerror("CryptAcquireContext failed in call to entropy.");
    return FALSE;
  }
  *rndbuf = malloc(len);
  nullbuf = malloc(nblen);
  for (i = 0; i < 128; i++) {
    if (!CryptGenRandom(provhnd, nblen, nullbuf)) {
      free(*rndbuf);
      *rndbuf = NULL;
      i=128;
    }
  }
  free(nullbuf);
  if (*rndbuf && !CryptGenRandom(provhnd, len, *rndbuf)) {
    free(*rndbuf);
    *rndbuf = NULL;
  }
  CryptReleaseContext(provhnd, 0);
  return *rndbuf ? TRUE : FALSE;
}

