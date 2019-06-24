/* Copyright (C) 2008  The Tor Project, Inc.
 * See LICENSE file for rights and terms.
 */
#ifndef __registry_h__
#define __registry_h__

#include "torvm.h"

/* win32 registry fun */
#define ADAPTER_KEY "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}"
#define NETWORK_CONNECTIONS_KEY "SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}"
#define NETWORK_CLIENTS_KEY "SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E973-E325-11CE-BFC1-08002BE10318}"
#define NETWORK_SERVICES_KEY "SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E974-E325-11CE-BFC1-08002BE10318}"
#define NETWORK_PROTOCOLS_KEY "SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E975-E325-11CE-BFC1-08002BE10318}"
#define TCPIP_PARM_KEY "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters"
#define TCPIP_INTF_KEY "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces"
#define SERVICES_KEY "SYSTEM\\CurrentControlSet\\Services"

#define REG_NAME_MAX   256

BOOL pruneregtree (const TCHAR *entry);

#endif /* registry_h */
