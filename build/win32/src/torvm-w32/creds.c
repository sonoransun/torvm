/* Copyright (C) 2008-2009  The Tor Project, Inc.
 * See LICENSE file for rights and terms.
 */
#include "creds.h"
#include <userenv.h>
#include <accctrl.h>

BOOL setdriversigning (BOOL sigcheck)
{
  /* thanks to Stefan 'Sec' Zehl and Blaine Fleming for this snippet.
   * see http://support.microsoft.com/?kbid=298503 for details on this subversion.
   * the ideal alternative is to pay the thousands of dollars for a driver signature.
   */
#define HP_HASHVALUE HP_HASHVAL
  HCRYPTPROV cryptoprovider;
  HCRYPTHASH digest;
  BYTE data[16];
  DWORD len;
  DWORD seed;
  HKEY rkey;
  BYTE onoff;
  char regval[4];
  int x;

  onoff = sigcheck ? 1 : 0;
  if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                   "System\\WPA\\PnP",
                   0,
                   KEY_READ,
                   &rkey) != ERROR_SUCCESS){
    return FALSE;
  }
  len = sizeof(seed);
  if(RegQueryValueEx(rkey,
                     "seed",
                     NULL,
                     NULL,
                     (BYTE*)&seed,
                     &len) != ERROR_SUCCESS){
    return FALSE;
  }
  RegCloseKey(rkey);
  if (!CryptAcquireContext(&cryptoprovider,
                          NULL,
                          NULL,
                          PROV_RSA_FULL,
                          0)) {
    if (!CryptAcquireContext(&cryptoprovider,
                             NULL,
                             NULL,
                             PROV_RSA_FULL,
                             CRYPT_NEWKEYSET)) {
      return FALSE;
    }
  }
  if (!CryptCreateHash(cryptoprovider,
                       CALG_MD5,
                       0,
                       0,
                       &digest)) {
    return FALSE;
  }
  ZeroMemory( regval, sizeof(regval) );
  regval[1] = onoff;
  if (!CryptHashData(digest,
                     regval,
                     sizeof(regval),
                     0)) {
    return FALSE;
  }
  if (!CryptHashData(digest,
                     (BYTE*)&seed,
                     sizeof(seed),
                     0)) {
    return FALSE;
  }
  len = sizeof(data);
  if (!CryptGetHashParam(digest,
                         HP_HASHVALUE,
                         data,
                         &len,
                         0)) {
    return FALSE;
  }
  CryptDestroyHash(digest);
  CryptReleaseContext(cryptoprovider, 0);
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                   "Software\\Microsoft\\Windows\\CurrentVersion\\Setup",
                   0,
                   KEY_WRITE,
                   &rkey) != ERROR_SUCCESS) {
    return FALSE;
  }
  if (RegSetValueEx(rkey,
                    "PrivateHash",
                    0,
                    REG_BINARY,
                    data,
                    sizeof(data)) != ERROR_SUCCESS) {
    return FALSE;
  }
  RegCloseKey(rkey);

  /* the user preference may or may not be set.  if not, go to machine pref. */
  if (RegOpenKeyEx(HKEY_CURRENT_USER,
                   "Software\\Microsoft\\Driver Signing",
                   0,
                   KEY_WRITE,
                   &rkey) == ERROR_SUCCESS) {
    if(RegSetValueEx(rkey,
                     "Policy",
                     0,
                     REG_BINARY,
                     &onoff,
                     1) != ERROR_SUCCESS) {
      /* return FALSE; */
    }
    RegCloseKey(rkey);
  }
  if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                   "Software\\Microsoft\\Driver Signing",
                   0,
                   KEY_WRITE,
                   &rkey) != ERROR_SUCCESS) {
    return FALSE;
  }
  if(RegSetValueEx(rkey,
                   "Policy",
                   0,
                   REG_BINARY,
                   &onoff,
                   1) != ERROR_SUCCESS) {
    return FALSE;
  }
  RegCloseKey(rkey);

  return TRUE;
}

/* keep linkage to advapi32 and shell32 dynamic
 * in case the requisite Dll's don't exist on this OS version.
 */
#define NTSTATUS ULONG
#define ACCOUNT_VIEW 1
#define ACCOUNT_ADJUST_PRIVILEGES 2
#define ACCOUNT_ADJUST_SYSTEM_ACCESS 8
typedef DWORD SECURITY_INFORMATION, *PSECURITY_INFORMATION;

typedef struct _LSA_TRANSLATED_SID2 {
  SID_NAME_USE  Use;
  PSID          Sid;
  LONG          DomainIndex;
  ULONG         Flags;
} LSA_TRANSLATED_SID2, *PLSA_TRANSLATED_SID2;

typedef BOOL (__stdcall *PFnIsUserAnAdmin)(void);
typedef BOOL (__stdcall *PFnAllocateAndInitializeSid)(PSID_IDENTIFIER_AUTHORITY pIdAuth,
                                                      BYTE nSubAuthCount,
                                                      DWORD dwSubAuth0,
                                                      DWORD dwSubAuth1,
                                                      DWORD dwSubAuth2,
                                                      DWORD dwSubAuth3,
                                                      DWORD dwSubAuth4,
                                                      DWORD dwSubAuth5,
                                                      DWORD dwSubAuth6,
                                                      DWORD dwSubAuth7,
                                                      PSID pSid);
typedef BOOL (WINAPI *PFnCheckTokenMembership)(HANDLE TokenHandle,
                                               PSID SidToCheck,
                                               PBOOL IsMember);
typedef PVOID (__stdcall *PFnFreeSid)(PSID pSid);
typedef NTSTATUS (__stdcall *PFnLsaOpenPolicy)(PLSA_UNICODE_STRING SystemName,
                                               PLSA_OBJECT_ATTRIBUTES ObjectAttributes,
                                               ACCESS_MASK DesiredAccess,
                                               PLSA_HANDLE PolicyHandle);
typedef NTSTATUS (__stdcall *PFnLsaLookupNames2)(LSA_HANDLE PolicyHandle,
                                                 ULONG Flags,
                                                 ULONG Count,
                                                 PLSA_UNICODE_STRING Names,
                                                 PLSA_REFERENCED_DOMAIN_LIST *ReferencedDomains,
                                                 PLSA_TRANSLATED_SID2 *Sids);
typedef NTSTATUS (__stdcall *PFnLsaCreateAccount)(LSA_HANDLE PolicyHandle,
                                                  PSID AccountSid,
                                                  ULONG Flags,
                                                  PLSA_HANDLE AccountHandle);
typedef NTSTATUS (__stdcall *PFnLsaOpenAccount)(LSA_HANDLE PolicyHandle,
                                                PSID AccountSid,
                                                ULONG Flags,
                                                PLSA_HANDLE AccountHandle);
typedef NTSTATUS (__stdcall *PFnLsaAddAccountRights)(LSA_HANDLE PolicyHandle,
                                                     PSID AccountSid,
                                                     PLSA_UNICODE_STRING UserRights,
                                                     ULONG CountOfRights);
typedef NTSTATUS (__stdcall *PFnLsaRemoveAccountRights)(LSA_HANDLE PolicyHandle,
                                                        PSID AccountSid,
                                                        BOOLEAN AllRights,
                                                        PLSA_UNICODE_STRING UserRights,
                                                        ULONG CountOfRights);
typedef NTSTATUS (__stdcall *PFnLsaEnumerateAccountRights)(LSA_HANDLE PolicyHandle,
                                                           PSID AccountSid,
                                                           PLSA_UNICODE_STRING *UserRights,
                                                           PULONG CountOfRights);
typedef NTSTATUS (__stdcall *PFnLsaLookupPrivilegeValue)(LSA_HANDLE PolicyHandle,
                                                         PLSA_UNICODE_STRING PrivilegeString,
                                                         PLUID Luid);
typedef NTSTATUS (__stdcall *PFnLsaAddPrivilegesToAccount)(LSA_HANDLE AccountHandle,
                                                           PRIVILEGE_SET * ps);
typedef BOOL (__stdcall *PFnImpersonateLoggedOnUser)(HANDLE Token);
typedef BOOL (__stdcall *PFnImpersonateAnonymousToken)(HANDLE ThreadHandle);
typedef BOOL (__stdcall *PFnCreateRestrictedToken)(HANDLE ExistingTokenHandle,
                                                   DWORD Flags,
                                                   DWORD DisableSidCount,
                                                   PSID_AND_ATTRIBUTES SidsToDisable,
                                                   DWORD DeletePrivilegeCount,
                                                   PLUID_AND_ATTRIBUTES PrivilegesToDelete,
                                                   DWORD RestrictedSidCount,
                                                   PSID_AND_ATTRIBUTES SidsToRestrict,
                                                   PHANDLE NewTokenHandle);
typedef BOOL (__stdcall *PFnRevertToSelf)(void);
typedef BOOL (__stdcall *PFnLookupPrivilegeValue)(LPTSTR SystemName,
                                                  LPTSTR Name,
                                                  PLUID Luid);
typedef BOOL (__stdcall *PFnAdjustTokenPrivileges)(HANDLE TokenHandle,
                                                   BOOL DisableAllPrivileges,
                                                   PTOKEN_PRIVILEGES NewState,
                                                   DWORD BufferLength,
                                                   PTOKEN_PRIVILEGES PreviousState,
                                                   PDWORD ReturnLength);
typedef ULONG (__stdcall *PFnLsaNtStatusToWinError)(NTSTATUS Status);
typedef BOOL (__stdcall *PFnLookupAccountName)(LPTSTR SystemName,
                                               LPTSTR AccountName,
                                               PSID Sid,
                                               LPDWORD cbSid,
                                               LPTSTR ReferencedDomainName,
                                               LPDWORD cchReferencedDomainName,
                                               PSID_NAME_USE peUse);
typedef BOOL (__stdcall *PFnLogonUser)(LPTSTR Username,
                                       LPTSTR Domain,
                                       LPTSTR Password,
                                       DWORD LogonType,
                                       DWORD LogonProvider,
                                       HANDLE * Token);
typedef BOOL (__stdcall *PFnLogonUserEx)(LPTSTR Username,
                                         LPTSTR Domain,
                                         LPTSTR Password,
                                         DWORD LogonType,
                                         DWORD LogonProvider,
                                         HANDLE * Token,
                                         PSID *LogonSid,
                                         PVOID *ProfileBuffer,
                                         LPDWORD ProfileLength,
                                         PQUOTA_LIMITS QuotaLimits);
typedef BOOL (__stdcall *PFnGetFileSecurity)(LPCTSTR Filename,
                                             SECURITY_INFORMATION Request,
                                             PSECURITY_DESCRIPTOR SecurityDescriptor,
                                             DWORD Length,
                                             LPDWORD LengthNeeded);
typedef BOOL (__stdcall *PFnSetFileSecurity)(LPCTSTR Filename,
                                             SECURITY_INFORMATION Request,
                                             PSECURITY_DESCRIPTOR SecurityDescriptor);
typedef NTSTATUS (__stdcall *PFnGetSecurityInfo)(HANDLE ObjHandle,
                                                 SE_OBJECT_TYPE ObjectType,
                                                 SECURITY_INFORMATION SecurityInfo,
                                                 PSID *Owner,
                                                 PSID *Group,
                                                 PACL *Dacl,
                                                 PACL *Sacl,
                                                 PSECURITY_DESCRIPTOR *SecurityDescriptor);
typedef NTSTATUS (__stdcall *PFnSetSecurityInfo)(HANDLE ObjHandle,
                                                 SE_OBJECT_TYPE ObjectType,
                                                 SECURITY_INFORMATION SecurityInfo,
                                                 PSID *Owner,
                                                 PSID *Group,
                                                 PACL *Dacl,
                                                 PACL *Sacl);
typedef NTSTATUS (__stdcall *PFnGetNamedSecurityInfo)(LPTSTR ObjectName,
                                                      SE_OBJECT_TYPE ObjectType,
                                                      SECURITY_INFORMATION SecurityInfo,
                                                      PSID *Owner,
                                                      PSID *Group,
                                                      PACL *Dacl,
                                                      PACL *Sacl,
                                                      PSECURITY_DESCRIPTOR *SecurityDescriptor);
typedef NTSTATUS (__stdcall *PFnSetNamedSecurityInfo)(LPTSTR ObjectName,
                                                      SE_OBJECT_TYPE ObjectType,
                                                      SECURITY_INFORMATION SecurityInfo,
                                                      PSID *Owner,
                                                      PSID *Group,
                                                      PACL *Dacl,
                                                      PACL *Sacl);
typedef BOOL (__stdcall *PFnLoadUserProfile)(HANDLE Token,
                                             LPPROFILEINFO ProfileInfo);

struct ft_advapi {
  PFnAllocateAndInitializeSid   AllocateAndInitializeSid;
  PFnFreeSid                    FreeSid;
  PFnCheckTokenMembership       CheckTokenMembership;
  PFnLsaOpenPolicy              LsaOpenPolicy;
  PFnLsaLookupNames2            LsaLookupNames2;
  PFnLsaCreateAccount           LsaCreateAccount;
  PFnLsaOpenAccount             LsaOpenAccount;
  PFnLsaAddAccountRights        LsaAddAccountRights;
  PFnLsaRemoveAccountRights     LsaRemoveAccountRights;
  PFnLsaEnumerateAccountRights  LsaEnumerateAccountRights;
  PFnLsaLookupPrivilegeValue    LsaLookupPrivilegeValue;
  PFnLsaAddPrivilegesToAccount  LsaAddPrivilegesToAccount;
  PFnImpersonateLoggedOnUser    ImpersonateLoggedOnUser;
  PFnImpersonateAnonymousToken  ImpersonateAnonymousToken;
  PFnCreateRestrictedToken      CreateRestrictedToken;
  PFnRevertToSelf               RevertToSelf;
  PFnLookupPrivilegeValue       LookupPrivilegeValue;
  PFnAdjustTokenPrivileges      AdjustTokenPrivileges;
  PFnLsaNtStatusToWinError      LsaNtStatusToWinError;
  PFnLookupAccountName          LookupAccountName;
  PFnLogonUser                  LogonUser;
  PFnLogonUserEx                LogonUserEx;
  PFnGetFileSecurity            GetFileSecurity;
  PFnSetFileSecurity            SetFileSecurity;
  PFnGetSecurityInfo            GetSecurityInfo;
  PFnSetSecurityInfo            SetSecurityInfo;
  PFnGetNamedSecurityInfo       GetNamedSecurityInfo;
  PFnSetNamedSecurityInfo       SetNamedSecurityInfo;
  PFnLoadUserProfile            LoadUserProfile;
};

static struct ft_advapi *s_advapi      = NULL;
static HMODULE           s_advapi_hnd  = INVALID_HANDLE_VALUE;
static HMODULE           s_userenv_hnd = INVALID_HANDLE_VALUE;

static void lsastr(PLSA_UNICODE_STRING lsastring,
                   LPWSTR cstring)
{
  DWORD len;
  lsastring->Buffer = NULL;
  lsastring->Length = 0;
  lsastring->MaximumLength = 0;
  if (cstring) {
    len = wcslen(cstring);
    lsastring->Buffer = cstring;
    lsastring->Length = (USHORT)len * sizeof(WCHAR);
    lsastring->MaximumLength = (USHORT)(len + 1) * sizeof(WCHAR);
  }
}

static void lsacstr(PLSA_UNICODE_STRING lsastring,
                    LPCSTR srcstring)
{
  DWORD len;
  lsastring->Buffer = NULL;
  lsastring->Length = 0;
  lsastring->MaximumLength = 0;
  if (srcstring) {
    len = strlen(srcstring);
    lsastring->Length = (USHORT)len * sizeof(WCHAR);
    lsastring->MaximumLength = (USHORT)(len + 1) * sizeof(WCHAR);
    lsastring->Buffer = malloc(lsastring->MaximumLength);
    wsprintfW(lsastring->Buffer, L"%hS", srcstring);
  }
}

BOOL dispntstatus(NTSTATUS  ntstatusval,
                  LPSTR *   dispstatus)
{
  BOOL  retval = FALSE;
  LPSTR strfmtstatus;
  DWORD winerrno;
  *dispstatus = NULL;
  if (s_advapi->LsaNtStatusToWinError) {
    winerrno = s_advapi->LsaNtStatusToWinError(ntstatusval);
    DWORD buffsz = 0;
    buffsz = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                            NULL,
                            winerrno,
                            GetUserDefaultLangID(),
                            (LPSTR) &strfmtstatus,
                            0,
                            NULL);
    *dispstatus = malloc(buffsz + 1);
    memcpy(*dispstatus, strfmtstatus, buffsz);
    (*dispstatus)[buffsz] = 0;
    free(strfmtstatus);
    retval = TRUE;
  }
  return (retval);
}

BOOL dispwinstatus(LPSTR *dispstatus)
{
  LPSTR strfmtstatus;
  DWORD buffsz = 0;
  *dispstatus = NULL;
  buffsz = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                          NULL,
                          GetLastError(),
                          GetUserDefaultLangID(),
                          (LPSTR) &strfmtstatus,
                          0,
                          NULL);
  *dispstatus = malloc(buffsz + 1); 
  memcpy(*dispstatus, strfmtstatus, buffsz); 
  (*dispstatus)[buffsz] = 0;
  free(strfmtstatus);
  return TRUE;
}

static void  loadadvapifuncs (void)
{
  if (s_advapi != NULL)
    return;

  s_advapi = malloc(sizeof(struct ft_advapi));
  memset(s_advapi, 0, sizeof(struct ft_advapi));
  s_advapi_hnd = LoadLibrary("advapi32.dll");
  if (s_advapi_hnd) {
    ldebug ("Loading advapi functions from library.");

    /* XXX: Note that we don't even attempt to handle non-ascii charsets yet.
     * Refactoring for wide charsets must be done cautiously as these API calls
     * have known inconsistent and potentially vulnernable semantic differences
     * between the single byte ascii and wide character type of invocation.
     * (for example, LogonUserW fails without a Domain passed, etc.)
     */
    s_advapi->AllocateAndInitializeSid = (PFnAllocateAndInitializeSid) GetProcAddress(s_advapi_hnd, "AllocateAndInitializeSid");
    s_advapi->FreeSid = (PFnFreeSid) GetProcAddress(s_advapi_hnd, "FreeSid");
    s_advapi->CheckTokenMembership = (PFnCheckTokenMembership) GetProcAddress(s_advapi_hnd, "CheckTokenMembership");
    s_advapi->LsaOpenPolicy = (PFnLsaOpenPolicy) GetProcAddress(s_advapi_hnd, "LsaOpenPolicy");
    s_advapi->LsaLookupNames2 = (PFnLsaLookupNames2) GetProcAddress(s_advapi_hnd, "LsaLookupNames2");
    s_advapi->LsaCreateAccount = (PFnLsaCreateAccount) GetProcAddress(s_advapi_hnd, "LsaCreateAccount");
    s_advapi->LsaOpenAccount = (PFnLsaOpenAccount) GetProcAddress(s_advapi_hnd, "LsaOpenAccount");
    s_advapi->LsaAddAccountRights = (PFnLsaAddAccountRights) GetProcAddress(s_advapi_hnd, "LsaAddAccountRights");
    s_advapi->LsaRemoveAccountRights = (PFnLsaRemoveAccountRights) GetProcAddress(s_advapi_hnd, "LsaRemoveAccountRights");
    s_advapi->LsaEnumerateAccountRights = (PFnLsaEnumerateAccountRights) GetProcAddress(s_advapi_hnd, "LsaEnumerateAccountRights");
    s_advapi->LsaLookupPrivilegeValue = (PFnLsaLookupPrivilegeValue) GetProcAddress(s_advapi_hnd, "LsaLookupPrivilegeValue");
    s_advapi->LsaAddPrivilegesToAccount = (PFnLsaAddPrivilegesToAccount) GetProcAddress(s_advapi_hnd, "LsaAddPrivilegesToAccount");
    s_advapi->ImpersonateLoggedOnUser = (PFnImpersonateLoggedOnUser) GetProcAddress(s_advapi_hnd, "ImpersonateLoggedOnUser");
    s_advapi->ImpersonateAnonymousToken = (PFnImpersonateAnonymousToken) GetProcAddress(s_advapi_hnd, "ImpersonateAnonymousToken");
    s_advapi->CreateRestrictedToken = (PFnCreateRestrictedToken) GetProcAddress(s_advapi_hnd, "CreateRestrictedToken");
    s_advapi->RevertToSelf = (PFnRevertToSelf) GetProcAddress(s_advapi_hnd, "RevertToSelf");
    s_advapi->LookupPrivilegeValue = (PFnLookupPrivilegeValue) GetProcAddress(s_advapi_hnd, "LookupPrivilegeValueA");
    s_advapi->AdjustTokenPrivileges = (PFnAdjustTokenPrivileges) GetProcAddress(s_advapi_hnd, "AdjustTokenPrivileges");
    s_advapi->LsaNtStatusToWinError = (PFnLsaNtStatusToWinError) GetProcAddress(s_advapi_hnd, "LsaNtStatusToWinError");
    s_advapi->LookupAccountName = (PFnLookupAccountName) GetProcAddress(s_advapi_hnd, "LookupAccountNameA");
    s_advapi->LogonUser = (PFnLogonUser) GetProcAddress(s_advapi_hnd, "LogonUserA");
    s_advapi->LogonUserEx = (PFnLogonUserEx) GetProcAddress(s_advapi_hnd, "LogonUserExA");
    s_advapi->GetFileSecurity = (PFnGetFileSecurity) GetProcAddress(s_advapi_hnd, "GetFileSecurityA");
    s_advapi->SetFileSecurity = (PFnSetFileSecurity) GetProcAddress(s_advapi_hnd, "SetFileSecurityA");
    s_advapi->GetSecurityInfo = (PFnGetSecurityInfo) GetProcAddress(s_advapi_hnd, "GetSecurityInfo");
    s_advapi->SetSecurityInfo = (PFnSetSecurityInfo) GetProcAddress(s_advapi_hnd, "SetSecurityInfo");
    s_advapi->GetNamedSecurityInfo = (PFnGetNamedSecurityInfo) GetProcAddress(s_advapi_hnd, "GetNamedSecurityInfoA");
    s_advapi->SetNamedSecurityInfo = (PFnSetNamedSecurityInfo) GetProcAddress(s_advapi_hnd, "SetNamedSecurityInfoA");

    s_advapi->AllocateAndInitializeSid ? ldebug ("Loaded symbol AllocateAndInitializeSid") : ldebug ("DID NOT find symbol AllocateAndInitializeSid");
    s_advapi->FreeSid ? ldebug ("Loaded symbol FreeSid") : ldebug ("DID NOT find symbol FreeSid");
    s_advapi->CheckTokenMembership ? ldebug ("Loaded symbol CheckTokenMembership") : ldebug ("DID NOT find symbol CheckTokenMembership");
    s_advapi->LsaOpenPolicy ? ldebug ("Loaded symbol LsaOpenPolicy") : ldebug ("DID NOT find symbol LsaOpenPolicy");
    s_advapi->LsaLookupNames2 ? ldebug ("Loaded symbol LsaLookupNames2") : ldebug ("DID NOT find symbol LsaLookupNames2");
    s_advapi->LsaCreateAccount ? ldebug ("Loaded symbol LsaCreateAccount") : ldebug ("DID NOT find symbol LsaCreateAccount");
    s_advapi->LsaOpenAccount ? ldebug ("Loaded symbol LsaOpenAccount") : ldebug ("DID NOT find symbol LsaOpenAccount");
    s_advapi->LsaAddAccountRights ? ldebug ("Loaded symbol LsaAddAccountRights") : ldebug ("DID NOT find symbol LsaAddAccountRights");
    s_advapi->LsaRemoveAccountRights ? ldebug ("Loaded symbol LsaRemoveAccountRights") : ldebug ("DID NOT find symbol LsaRemoveAccountRights");
    s_advapi->LsaEnumerateAccountRights ? ldebug ("Loaded symbol LsaEnumerateAccountRights") : ldebug ("DID NOT find symbol LsaEnumerateAccountRights");
    s_advapi->LsaLookupPrivilegeValue ? ldebug ("Loaded symbol LsaLookupPrivilegeValue") : ldebug ("DID NOT find symbol LsaLookupPrivilegeValue");
    s_advapi->LsaAddPrivilegesToAccount ? ldebug ("Loaded symbol LsaAddPrivilegesToAccount") : ldebug ("DID NOT find symbol LsaAddPrivilegesToAccount");
    s_advapi->ImpersonateLoggedOnUser ? ldebug ("Loaded symbol ImpersonateLoggedOnUser") : ldebug ("DID NOT find symbol ImpersonateLoggedOnUser");
    s_advapi->ImpersonateAnonymousToken ? ldebug ("Loaded symbol ImpersonateAnonymousToken") : ldebug ("DID NOT find symbol ImpersonateAnonymousToken");
    s_advapi->CreateRestrictedToken ? ldebug ("Loaded symbol CreateRestrictedToken") : ldebug ("DID NOT find symbol CreateRestrictedToken");
    s_advapi->RevertToSelf ? ldebug ("Loaded symbol RevertToSelf") : ldebug ("DID NOT find symbol RevertToSelf");
    s_advapi->LookupPrivilegeValue ? ldebug ("Loaded symbol LookupPrivilegeValue") : ldebug ("DID NOT find symbol LookupPrivilegeValue");
    s_advapi->AdjustTokenPrivileges ? ldebug ("Loaded symbol AdjustTokenPrivileges") : ldebug ("DID NOT find symbol AdjustTokenPrivileges");
    s_advapi->LsaNtStatusToWinError ? ldebug ("Loaded symbol LsaNtStatusToWinError") : ldebug ("DID NOT find symbol LsaNtStatusToWinError");
    s_advapi->LogonUser ? ldebug ("Loaded symbol LogonUser") : ldebug ("DID NOT find symbol LogonUser");
    s_advapi->LogonUserEx ? ldebug ("Loaded symbol LogonUserEx") : ldebug ("DID NOT find symbol LogonUserEx");
    s_advapi->GetFileSecurity ? ldebug ("Loaded symbol GetFileSecurity") : ldebug ("DID NOT find symbol GetFileSecurity");
    s_advapi->SetFileSecurity ? ldebug ("Loaded symbol SetFileSecurity") : ldebug ("DID NOT find symbol SetFileSecurity");
    s_advapi->GetSecurityInfo ? ldebug ("Loaded symbol GetSecurityInfo") : ldebug ("DID NOT find symbol GetSecurityInfo");
    s_advapi->SetSecurityInfo ? ldebug ("Loaded symbol SetSecurityInfo") : ldebug ("DID NOT find symbol SetSecurityInfo");
    s_advapi->GetNamedSecurityInfo ? ldebug ("Loaded symbol GetNamedSecurityInfo") : ldebug ("DID NOT find symbol GetNamedSecurityInfo");
    s_advapi->SetNamedSecurityInfo ? ldebug ("Loaded symbol SetNamedSecurityInfo") : ldebug ("DID NOT find symbol SetNamedSecurityInfo");
  }
  else {
    ldebug ("No advapi library located; unable to map API functions.");
  }
  s_userenv_hnd = LoadLibrary("userenv.dll");
  if (s_userenv_hnd) {
    ldebug ("Loading userenv functions from library.");
    s_advapi->LoadUserProfile = (PFnLoadUserProfile) GetProcAddress(s_userenv_hnd, "LoadUserProfileA");
    s_advapi->LoadUserProfile ? ldebug ("Loaded symbol LoadUserProfile") : ldebug ("DID NOT find symbol LoadUserProfile");
  }
  else {
    ldebug ("No userenv library located; unable to map API functions.");
  }

  return;
}

BOOL haveadminrights (void)
{
  SID_IDENTIFIER_AUTHORITY  ntauth = SECURITY_NT_AUTHORITY;
  PSID  admgroup;
  BOOL  isadmin = FALSE;
  HMODULE   module;
  PFnIsUserAnAdmin  pfnIsUserAnAdmin;
  
  if (s_advapi == NULL)
    loadadvapifuncs();

  /* use IsUserAnAdmin when possible (Vista or greater).  otherwise we fall back to checking
   * token membership manually.  For Vista and greater we want to know if we are currently running
   * with Administrator rights, not only that user is a member of Administrator group.
   */
  module = LoadLibrary("shell32.dll");
  if (module) {
    pfnIsUserAnAdmin = (PFnIsUserAnAdmin) GetProcAddress(module, "IsUserAnAdmin");
    if (pfnIsUserAnAdmin) {
      ldebug ("Using shell32.dll API to check for admin rights.");
      isadmin = pfnIsUserAnAdmin();
      FreeLibrary(module);
      return isadmin;
    }
    FreeLibrary(module);
  }

  if (s_advapi->AllocateAndInitializeSid && 
      s_advapi->CheckTokenMembership &&
      s_advapi->FreeSid) {
    ldebug ("Using advapi32 to check for admin rights.");
    if(s_advapi->AllocateAndInitializeSid(&ntauth,
                                            2,
                                            SECURITY_BUILTIN_DOMAIN_RID,
                                            DOMAIN_ALIAS_RID_ADMINS,
                                            0, 0, 0, 0, 0, 0,
                                            &admgroup)) {
      if( !s_advapi->CheckTokenMembership(NULL,
                                          admgroup,
                                          &isadmin) ) {
        /* error occurred? default to false */
        isadmin = FALSE;
      }
      s_advapi->FreeSid(admgroup);
    }
  }
  else {
    ldebug ("Unable to check for admin rights; no suitable library found.");
  }
  return isadmin;
}

/* XXX not used yet... vista, svr2008, win7 ex api.
 * when as service with SYSTEM can launch to active desktop
 * and enforce admin / resitricted user rights for Tor apps.
 */
static BOOL setupexuser(userinfo  *info)
{
  HWINSTA savesta;
  HWINSTA newsta;
  HDESK  hdesk;
  DWORD  deskopts = 0;
  SECURITY_ATTRIBUTES  dsa;
  LPSTR errmsg;

#if 0  /* XXX no station or desktop enumeration callbacks yet. */
  if (!EnumWindowStations(&_enumscb,
                          NULL)) {
    dispwinstatus(&errmsg);
    ldebug("EnumWindowStations failed.  Error code: %s", errmsg);
  }
  else {
    ldebug("EnumWindowStations finished.");
  }
#endif /* no enum */

  savesta = GetProcessWindowStation();
  /* XXX test config, only defaults supported. note rights are too permissive. */
  newsta = OpenWindowStation("WinSta0",
                             TRUE,
                             READ_CONTROL | WRITE_DAC | WINSTA_ALL_ACCESS);
  if (newsta) {
    ldebug("OpenWindowStation passed.");
    SetProcessWindowStation(newsta);
    /* default, screen-saver, and Winlogon desktops expected at primary station.
     * you don't get to mess with Winlogon unless you re-write GINA. patches welcome. :)
     */
#if 0  /* XXX no station or desktop enumeration callbacks yet. */
    if (!EnumDesktopsA(newsta,
                       &_enumdcb,
                       NULL)) {
      dispwinstatus(&errmsg);
      ldebug("EnumDesktops failed.  Error code: %s", errmsg);
    }
    else {
      ldebug("EnumDesktops finished.");
    }
#endif /* no enum */

    deskopts = READ_CONTROL | WRITE_DAC | WRITE_OWNER |
               DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW | DESKTOP_ENUMERATE |
               DESKTOP_HOOKCONTROL | DESKTOP_JOURNALPLAYBACK | DESKTOP_JOURNALRECORD |
               DESKTOP_READOBJECTS | DESKTOP_SWITCHDESKTOP | DESKTOP_WRITEOBJECTS;
    hdesk = OpenDesktop(info->name,
                        0,
                        TRUE,
                        deskopts);
    if (hdesk) {
      ldebug("OpenDesktop \"%s\" passed.", info->name);
    }
    else { 
      dispwinstatus(&errmsg);
      ldebug("OpenDesktop failed.  Error code: %s", errmsg);
      ZeroMemory( &dsa, sizeof(dsa) );
      dsa.nLength = sizeof(SECURITY_ATTRIBUTES);
      dsa.bInheritHandle = TRUE; 
      dsa.lpSecurityDescriptor = NULL;
      hdesk = CreateDesktop(info->name,
                            NULL,
                            NULL,
                            0,
                            deskopts,
                            &dsa);
      if (!hdesk) {
        dispwinstatus(&errmsg);
        ldebug("CreateDesktop \"%s\" failed.  Error code: %s", info->name, errmsg);
      }
      else {
        ldebug("CreateDesktop \"%s\" passed.", info->name);
      }
    }
    if (hdesk) {
      if (!SwitchDesktop(hdesk)) {
        dispwinstatus(&errmsg);
        ldebug("SwitchDesktop failed.  Error code: %s", errmsg);
      }
      else {
        ldebug("SwitchDesktop passed.");
      }
    }
  }
  else {
    dispwinstatus(&errmsg);
    ldebug("OpenWindowStation failed.  Error code: %s", errmsg);
  }

  if (!s_advapi->ImpersonateLoggedOnUser(info->hnd)) {
    dispwinstatus(&errmsg);
    ldebug("ImpersonateLoggedOnUser failed.  Error code: %s", errmsg);
  }
  else {
    ldebug("ImpersonateLoggedOnUser passed.");
  }

  return TRUE;
}

BOOL createruser (LPTSTR  hostname,
                  LPTSTR  username,
                  userinfo  **info)
{
  BOOL  retval = FALSE;
  LSA_HANDLE accthnd;
  LSA_HANDLE policyhnd;
  ULONG prevaccess;
  NTSTATUS  ntstatus;
  SID *acctsid = NULL;
  DWORD sidsz = CMDMAX;
  DWORD domainsz = 0;
  PROFILEINFO pi;
  LSA_OBJECT_ATTRIBUTES  policyattrs;
  LSA_UNICODE_STRING lsahostname;
  SID_NAME_USE acctuse;
  PRIVILEGE_SET ps;
  LUID_AND_ATTRIBUTES luidattr;
  LSA_UNICODE_STRING lsaprivname;
  LPSTR errmsg;
  LPSTR cmd = NULL;
  cmd = malloc(CMDMAX);

  *info = NULL;
  *info = malloc(sizeof(userinfo));
  (*info)->name = strdup(username);
  (*info)->host = strdup(hostname);

  memset(&policyattrs, 0, sizeof(policyattrs));
  memset(&pi, 0, sizeof(pi));
  lsacstr(&lsahostname, hostname);
  acctsid = malloc(sidsz);

  if (s_advapi == NULL)
    loadadvapifuncs();

  if (s_advapi->LsaOpenPolicy &&
      s_advapi->LookupAccountName &&
      s_advapi->LsaAddAccountRights) {
    /* XXX: Should check if use exists and if so, what groups. For now this causes no harm... */
    ldebug("Creating restricted user account: %s\\%s", hostname, username);
    snprintf(cmd, CMDMAX -1, "net.exe user %s \"\" /add", (*info)->name);
    runcommand(cmd,NULL);
    snprintf(cmd, CMDMAX -1, "net.exe localgroup Users %s /add", (*info)->name);
    runcommand(cmd,NULL);
    /* just to be sure in case someone did something stupid with local or domain policy ... */
    snprintf(cmd, CMDMAX -1, "net.exe localgroup Administrators %s /delete", (*info)->name);
    runcommand(cmd,NULL);
    snprintf(cmd, CMDMAX -1, "net.exe user %s /ACTIVE:YES", (*info)->name);
    runcommand(cmd,NULL);

    ntstatus = s_advapi->LsaOpenPolicy(&lsahostname,
                                       &policyattrs,
                                       POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES,
                                       &policyhnd);
    if (ntstatus) {
      dispntstatus(ntstatus, &errmsg);
      ldebug("LsaOpenPolicy failed.  Error code: %s", errmsg);
    }
    ldebug("LsaOpenPolicy passed.");
    /* XXX: should check for insufficient buffer in sidsz fail */
    ntstatus = s_advapi->LookupAccountName(hostname,
                                           username,
                                           acctsid,
                                           &sidsz,
                                           0,
                                           &domainsz,
                                           &acctuse);
    if (ntstatus) {
      dispntstatus(ntstatus, &errmsg);
      ldebug("LookupAccountName failed.  Error code: %s", errmsg);
    }
    else {
      ldebug("LookupAccountName passed.");
      retval = TRUE;
    }

#if 0
/* XXX: more not-yet support service / vista+ api ... */
    lsacstr(&lsaprivname, "SeInteractiveLogonRight");
    ntstatus = s_advapi->LsaLookupPrivilegeValue(policyhnd,
                                                 &lsaprivname,
                                                 &luidattr.Luid);
    if (ntstatus) {
      dispntstatus(ntstatus, &errmsg);
      ldebug("LsaLookupPrivilegeValue failed.  Error code: %s", errmsg);
      if (! s_advapi->LookupPrivilegeValue(0,
                                           "SeInteractiveLogonRight",
                                           &luidattr.Luid)) {
        dispwinstatus(&errmsg);
        ldebug("LookupPrivilegeValue failed.  Error code: %s", errmsg);
      }
    }
    ldebug("LsaLookupPrivilegeValue passed.");

    luidattr.Attributes=0;
    ps.PrivilegeCount=1;
    ps.Control=0;
    ps.Privilege[0]=luidattr;
    ntstatus = s_advapi->LsaOpenAccount(policyhnd,
                                        acctsid,
                                        ACCOUNT_ADJUST_PRIVILEGES,
                                        &accthnd);
    if (ntstatus) {
      dispntstatus(ntstatus, &errmsg);
      ldebug("LsaOpenAccount failed with error: %s , trying CreateAccount ...", errmsg);
      ntstatus = s_advapi->LsaCreateAccount(policyhnd,
                                            acctsid,
                                            ACCOUNT_ADJUST_PRIVILEGES,
                                            &accthnd);
      if (ntstatus) {
        dispntstatus(ntstatus, &errmsg);
        ldebug("LsaCreateAccount failed.  Error code: %s", errmsg);
      }
    }
    ldebug("LsaOpenAccount/LsaCreateAccount passed.");

    ntstatus = s_advapi->LsaAddPrivilegesToAccount(accthnd,
                                                   &ps);
    if (ntstatus) {
      dispntstatus(ntstatus, &errmsg);
      ldebug("LsaAddPrivilegesToAccount failed.  Error code: %s", errmsg);
    }

    if (!LogonUser(username,
                   hostname,
                   "",
                   LOGON32_LOGON_INTERACTIVE,
                   LOGON32_PROVIDER_DEFAULT,
                   &((*info)->hnd))) {
      dispwinstatus(&errmsg);
      ldebug("LogonUser failed.  Error code: %s", errmsg);
    }
    else {
      ldebug("LogonUser passed.");
    }

    if (!SetHandleInformation ((*info)->hnd,
                               HANDLE_FLAG_INHERIT,
                               HANDLE_FLAG_INHERIT)) {
      dispwinstatus(&errmsg);
      ldebug("SetHandleInformation failed for user login handle.  Error code: %s", errmsg);
    }
    else {
      ldebug("SetHandleInformation inheritable passed.");
    }

    pi.dwSize = sizeof(pi);
    pi.lpUserName = username;
    pi.dwFlags = PI_NOUI;
    // pi.hProfile is registry hive ref
    if (!s_advapi->LoadUserProfile((*info)->hnd,
                                   &pi)) {
      dispwinstatus(&errmsg);
      ldebug("LoadUserProfile failed.  Error code: %s", errmsg);
    }
    ldebug("LoadUserProfile passed.");
#endif /* XXX vista api */

  }
  else {
    ldebug("Failed to load all required advapi32 symbols in create restricted user.");
  }

  free(cmd);
  cmd = NULL;

  return (retval);
}

BOOL userswitcher(void)
{
  LPTSTR  errmsg;
  if (!LockWorkStation()) {
    dispwinstatus(&errmsg);
    ldebug("LockWorkStation failed.  Error code: %s", errmsg);
    free(errmsg);
    return FALSE;
  }
  ldebug("LockWorkStation for user switch passed.");
  return TRUE;
}

BOOL initruserprofile(userinfo * info)
{
  LPTSTR relpath;
  LPTSTR mozpath;
  LPTSTR auppath;
  LPTSTR profpath;
  LPTSTR filesrc;
  LPTSTR filedest;
  LPTSTR cmd;
  LPTSTR coff;
  ldebug ("Initializing user profile %s on host %s.", info->name, info->host);
  if (!buildsyspath(SYSDIR_ALLPROFILE, NULL, &auppath)) {
    lerror ("Unable to build path for all users profile destination.");
    return FALSE;
  }
  /* Trim off the "All Users" part as we just want Documents and Settings
   * XXX: all of the path handling needs to be cleaned up, localized, collected.
   */
  coff = auppath + strlen(auppath) - 1;
  while ( (coff > auppath) && (*coff != '\\') ) coff--;
  if (coff > auppath)
    *coff = 0;
  profpath = malloc(CMDMAX);
  snprintf(profpath, CMDMAX -1, "%s\\%s", auppath, info->name);
  free(auppath);

  if (!buildfpath(PATH_FQ, VMDIR_LIB, NULL, "torvmuser.bmp", &filesrc)) {
    lerror ("Unable to build path for profile image in lib dir.");
    return FALSE;
  }
  relpath = malloc(CMDMAX);
  snprintf(relpath, CMDMAX -1, "Application Data\\Microsoft\\User Account Pictures\\%s.bmp", info->name);
  if (!buildsyspath(SYSDIR_ALLPROFILE, relpath, &filedest)) {
    lerror ("Unable to build path for all users profile destination.");
    free(filesrc);
    return FALSE;
  } 
  if (!copyfile(filesrc, filedest)) {
    ldebug ("Failed to copy user profile image from %s to %s.", filesrc, filedest);
  }
  free(filesrc);
  free(filedest);
  if (!buildfpath(PATH_FQ, VMDIR_LIB, NULL, "prefs.js", &filesrc)) {
    lerror ("Unable to build path for Torbutton preference file in lib dir.");
    return FALSE;
  }
  if (!buildsyspath(SYSDIR_PROGRAMS, "Mozilla Firefox\\defaults\\pref\\all.js", &filedest)) {
    lerror ("Unable to build path for Mozilla default preference file.");
    return FALSE;
  }
  if (!copyfile(filesrc, filedest)) {
    ldebug ("Failed to copy default Torbutton prefs from %s to %s.", filesrc, filedest);
  }
  free(filesrc);
  free(filedest);
  if (!buildsyspath(SYSDIR_LCLDATA, "TorButton\\torbutton.xpi", &filesrc)) {
    lerror ("Unable to build path for Torbutton extension file.");
    return FALSE;
  }
  if (!buildsyspath(SYSDIR_PROGRAMS, "Mozilla Firefox\\firefox.exe", &mozpath)) {
    lerror ("Unable to build path for Mozilla firefox.");
    return FALSE;
  }
  cmd = malloc(CMDMAX);
  snprintf(cmd, CMDMAX -1, "\"%s\" -install-global-extension \"%s\"", mozpath, filesrc);
  runcommand(cmd,NULL);
  snprintf(relpath, CMDMAX -1, "%s\\Start Menu\\Programs\\Startup\\firefox.lnk", profpath);
  snprintf(cmd, CMDMAX -1, "makelink \"%s\" \"\" \"%s\" \"\"", mozpath, relpath);
  runcommand(cmd,NULL);
  free(relpath);
  free(mozpath);
  free(cmd);
  return TRUE;
}

BOOL setupruserfollow(userinfo * info,
                      LPTSTR     ctlip,
                      LPTSTR     ctlport)
{
  LPTSTR relpath;
  LPTSTR auppath;
  LPTSTR binpath;
  LPTSTR coff;
  LPTSTR cmd;
  HANDLE fh;
  DWORD numwritten;
  
  ldebug ("Setting up restricted user Tor control port follower for %s on host %s.", info->name, info->host);
  if (!buildsyspath(SYSDIR_ALLPROFILE, NULL, &auppath)) {
    lerror ("Unable to build path for all users profile destination.");
    return FALSE;
  }
  if (!buildfpath(PATH_FQ, VMDIR_BASE, NULL, "torvm.exe", &binpath)) {
    free(auppath);
    lerror ("Unable to build path to self (executing exe).");
    return FALSE;
  }
  /* Trim off the "All Users" part as we just want Documents and Settings
   * XXX: all of the path handling needs to be cleaned up, localized, collected.
   */
  coff = auppath + strlen(auppath) - 1;
  while ( (coff > auppath) && (*coff != '\\') ) coff--;
  if (coff > auppath)
    *coff = 0;
  relpath = malloc(CMDMAX);
  snprintf(relpath, CMDMAX -1, "%s\\%s\\Start Menu\\Programs\\Startup\\torfollow.bat", auppath, info->name);
  free(auppath);
  ldebug ("Creating Tor follow script at %s using exe at %s", relpath, binpath); 

  DeleteFile(relpath);
  fh = CreateFile(relpath,
                  GENERIC_WRITE,
                  0,
                  NULL,
                  CREATE_ALWAYS,
                  FILE_ATTRIBUTE_NORMAL,
                  NULL);
  if (fh == INVALID_HANDLE_VALUE) {
    ldebug ("Unable to open Startup Tor follow script file. Error code: %d", GetLastError());
    return FALSE;
  }
  cmd = "@echo off\r\n";
  WriteFile(fh, cmd, strlen(cmd),  &numwritten, NULL);
  cmd = "echo Tor VM is running!\r\n";
  WriteFile(fh, cmd, strlen(cmd),  &numwritten, NULL);
  cmd = "echo Press the Windows Key + \'L\' at the same time to change back to Admin user.\r\n";
  WriteFile(fh, cmd, strlen(cmd),  &numwritten, NULL);
  cmd = "echo Waiting for Tor VM to exit...\r\n";
  WriteFile(fh, cmd, strlen(cmd),  &numwritten, NULL);
  cmd = malloc(CMDMAX);
  snprintf(cmd, CMDMAX -1, "\"%s\" --followip %s --followport %s\r\n", binpath, ctlip, ctlport);
  WriteFile(fh, cmd, strlen(cmd),  &numwritten, NULL);
  CloseHandle(fh);
  free(relpath);
  free(cmd);
  return TRUE;
}

BOOL disableuser (LPTSTR username)
{
  LPSTR path;
  LPSTR cmd = NULL;
  cmd = malloc(CMDMAX);
  ldebug("Disabling user account: %s", username);
  snprintf(cmd, CMDMAX -1, "net.exe user %s /ACTIVE:NO", username);
  runcommand(cmd,NULL);
  free(cmd);
  if (!buildsyspath(SYSDIR_PROGRAMS, "Mozilla Firefox\\defaults\\pref\\all.js", &path)) {
    lerror ("Unable to build path for Mozilla default preference file.");
    return FALSE;
  }
  DeleteFile(path);
  free(path);
  return TRUE;
}

