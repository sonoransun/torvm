/* Copyright (C) 2008-2009  The Tor Project, Inc.
 * See LICENSE file for rights and terms.
 */
#ifndef __torvm_h__
#define __torvm_h__

#include "vmconfig.h"
#include "apicommon.h"
#include "creds.h"
#include "registry.h"
#include "thr.h"

typedef struct s_rconnelem {
  BOOL    isactive;
  BOOL    isdefgw;
  BOOL    isdhcp;
  BOOL    istortap;
  LPTSTR  name;
  LPTSTR  guid;
  LPTSTR  macaddr;
  LPTSTR  ipaddr;
  LPTSTR  netmask;
  LPTSTR  gateway;
  LPTSTR  gwmacaddr;
  LPTSTR  dhcpsvr;
  LPTSTR  svrmacaddr;
  LPTSTR  dhcpname;
  LPTSTR  dns1;
  LPTSTR  dns2;
  LPTSTR  driver;
  struct s_rconnelem * next;
} t_rconnelem;

typedef struct s_ctx {
  BOOL          vmaccel;
  BOOL          bundle;
  BOOL          usegeoip;
  BOOL          indebug;
  BOOL          vmnop;
  BOOL          noinit;
  BOOL          running;
  HANDLE        insthnd;
  LPTSTR        netcfgfile;
  LPTSTR        fwcfgfile;
  t_rconnelem * tapconn;
  t_rconnelem * brconn;
} t_ctx;

#define TORVM_INSTNAME "{1c6870d3-235d-4fb7-828d-25d7f05e2e76}"

#endif /* torvm_h */
