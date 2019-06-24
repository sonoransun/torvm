/* Copyright (C) 2008-2009  The Tor Project, Inc.
 * See LICENSE file for rights and terms.
 */
#ifndef __creds_h__
#define __creds_h__

#include "torvm.h"
#include <ntsecpkg.h>
#include <ntsecapi.h>

typedef struct s_userinfo {
  BOOL    isrestricted;
  BOOL    isadmin;
  BOOL    isinteractive;
  LPTSTR  name;
  LPTSTR  host;
  HANDLE  hnd;
  struct s_rconnelem * next;
} userinfo;

BOOL userswitcher (void);

/* We gotta have 'em! */
BOOL haveadminrights (void);

/* Avoid useless "Are you sure?" click through annoyance for the kernel drivers.
 * I claim usability veto power over M$ best intentions; the user is expected to
 * understand that this software is configuring network services by nature of
 * its form and function.
 */
BOOL setdriversigning (BOOL sigcheck);

/* Create or open restricted user account. */
BOOL createruser (LPTSTR  hostname,
                  LPTSTR  username,
                  userinfo **info);
BOOL disableuser (LPTSTR  username);

BOOL initruserprofile(userinfo *info);
                      

/* Generate RFC2440-style iterated and salted string-to-key for control port
 * Like other point-to-pointer arguments caller is responsible for free'ing
 * hashkey returned on successful invocation.
 */
BOOL passtokey (LPBYTE  password,
                LPBYTE  *hashkey);

#endif /* creds_h */
