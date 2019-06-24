#!/bin/bash
lf="target:busybox:LICENSE $lf"
lf="target:openssl:LICENSE $lf"
lf="target:e2fsprogs:COPYING $lf"
lf="target:iproute2:COPYING $lf"
lf="target:libupnp:LICENSE $lf"
lf="target:libtool:COPYING $lf"
lf="target:zlib:README $lf"
lf="linux:linux:COPYING $lf"
lf="linux:iptables:COPYING $lf"
lf="linux:tor:LICENSE $lf"
function usage () {
  if (( $# )); then
    echo "Error: $1" >&2
  fi
  echo 'Usage: genlicense.sh <kamikaze build dir> <license file tgz>' >&2
  exit 1
}
function extractlicense () {
  if (( $# != 5 )); then
    echo "ERROR: extractlicense invalid args." >&2
    return 1
  fi
  mysrcdir="$1"
  mysrcsubdir="$2"
  mysrcname="$3"
  myfilename="$4"
  mydestdir="$5"
  cp ${mysrcdir}/${mysrcsubdir}*/${mysrcname}*/${myfilename} "${mydestdir}/${mysrcname}-${myfilename}.txt"
  return $?
}

# start of script execution
if (( $# != 2 )); then
  usage
fi
dir="$1"
docfile="$2"
if [ ! -d "$dir" ]; then
  usage "Directory $dir does not exist."
fi
tmpdir=_lictmp
if [ -e $tmpdir ]; then
  rm -rf $tmpdir >/dev/null 2>&1
fi
mkdir $tmpdir
cp $dir/../LICENSE "${tmpdir}/OpenWRT-LICENSE.txt"
for pair in $lf; do
  sdir=${pair%%*:}
  sdir=${sdir/:*/}
  name=${pair#*:}
  name=${name/:*/}
  lfile=${pair##*:}
  extractlicense $dir $sdir $name $lfile $tmpdir
done
(
  cd $tmpdir
  tar zcf ../$docfile .
)
rm -rf $tmpdir >/dev/null 2>&1
if [ -f $docfile ]; then
  ls -l $docfile
  exit 0
fi
exit 1
