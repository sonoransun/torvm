#!/bin/bash
# Copyright (C) 2008-2009  The Tor Project, Inc.
# See LICENSE file for rights and terms.

if (( $# != 3 )); then
  echo "Usage: `basename $0` SrcURL SHA256 DestPath" >&2
  exit 1
fi
SRCURL="$1"
SUMEXPECTED="$2"
SAVEAS="$3"
DLTMP="${SAVEAS}.dltmp"

# get an sha1 digest using sha1sum or gpg and store in $SHA1OUT
export ZEROSHA256=e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
cmdsum () {
  sha256sum=`which sha256sum`
  if (( $? != 0 )); then
    return 1
  fi
  SHA256OUT=`$sha256sum "$1" | sed 's/ .*//'`
  return 0
}

gpgsum () {
  gpgbin=`which gpg`
  if (( $? != 0 )); then
    return 1
  fi
  SHA256OUT=`$gpgbin --print-md sha256 "$1" 2>/dev/null | tr -d '[:space:]' | sed 's/.*://' | sed 's/[^0-9a-fA-F]//g' | tr -t '[:upper:]' '[:lower:]'`
  return 0
}

dfunc=
cmdsum /dev/null
if (( $? == 0 )); then
  if [[ "$SHA256OUT" == "$ZEROSHA256" ]]; then
    dfunc=cmdsum
  fi
fi
if [ -z "$dfunc" ]; then
  gpgsum /dev/null
  if (( $? == 0 )); then
    if [[ "$SHA256OUT" == "$ZEROSHA256" ]]; then
      dfunc=gpgsum
    fi
  fi 
fi
if [ -z "$dfunc" ]; then
  echo "ERROR: Unable to find suitable sha256sum utility.  Please install sha256sum or gpg." >&2
  exit 1
fi

echo "Retrieving $SRCURL ..."
wget --no-check-certificate -t5 --timeout=30 $WGET_OPTIONS -O "$DLTMP" "$SRCURL"
if (( $? != 0 )); then
  echo "ERROR: Could not retrieve file $SRCURL" >&2
  if [ -f "$DLTMP" ]; then
    rm -f "$DLTMP"
  fi
  exit 1
fi
$dfunc "$DLTMP"
if [[ "$SHA256OUT" != "$SUMEXPECTED" ]]; then
  echo "ERROR: Digest for file `basename $DLTMP` does not match." >&2
  echo "       Expected $SUMEXPECTED but got $SHA256OUT instead." >&2
  rm -f "$DLTMP"
  exit 1
fi
mv "$DLTMP" "$SAVEAS"
echo "SHA-256 Digest verified OK for `basename $SAVEAS`"
exit 0
