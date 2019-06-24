#!/bin/bash
##
##  $Id: build-geoip-cache.sh 2362 2008-02-29 04:30:11Z edmanm $
## 
##  This file is part of Vidalia, and is subject to the license terms in the
##  LICENSE file, found in the top level directory of this distribution. If 
##  you did not receive the LICENSE file with this file, you may obtain it
##  from the Vidalia source package distributed by the Vidalia Project at
##  http://www.vidalia-project.net/. No part of Vidalia, including this file,
##  may be copied, modified, propagated, or distributed except according to
##  the terms described in the LICENSE file.
##


DIRURL="http://tor.noreply.org/tor/status/all"
GEOIPURL="http://peertech.org/geoip"
CACHEFILE="geoip-cache"

# Fetch a list of server IP addresses
ipaddrs=$(wget -q -O - "$DIRURL" | awk '$1 == "r" { print $7 }' | sort | uniq | tr "\n" ",")

# Get GeoIP information for each IP address
geoips=$(wget -q -O - --post-data="ip=$ipaddrs" "$GEOIPURL")

# Cache the GeoIP information with timestamps
IFS=$'\n'
rm -f $CACHEFILE
for geoip in $geoips; do
  echo "$geoip" >> "$CACHEFILE"
done

