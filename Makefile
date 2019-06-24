# Copyright (C) 2008-2009  The Tor Project, Inc.
# See LICENSE file for rights and terms.

SHELL=/bin/bash
export SHELL

ifeq (1,$(IN_KAMIKAZE_PRECACHE))
include build/kamikaze/common/pkg-cache.mk
default all: downloads
else
ifeq (1,$(IN_WIN32_PRECACHE))
include build/win32/Makefile
default all: downloads
else

# setup your own local defaults outside svn or dist files here
# include local.mk

# set various build defaults
ifeq (,$(BUSER))
	BUSER=guest
endif
ifeq (,$(BGROUP))
	BGROUP=users
endif
ifeq (,$(TGTNAME))
	TGTNAME=x86-vm
endif

# default locations for download directories
# vm kernel packages are distinct from win32 packages by nature of the
# two phase build process.
ifeq (,$(DLDIR))
	DLDIR=./build/kamikaze/common/dl
endif
ifeq (,$(WDLDIR))
	WDLDIR=./build/win32/dl
endif
ifeq (,$(SDLDIR))
	SDLDIR=./build/repos
endif
override DLDIR:=$(realpath $(DLDIR))
override WDLDIR:=$(realpath $(WDLDIR))
override SDLDIR:=$(realpath $(SDLDIR))

ifeq (,$(NO_PRECACHE))
PRECACHE_OPT=precache
endif

# OpenWRT version for build
override CWRTVER:=16018

export BUSER
export BGROUP
export TGTNAME
export DLDIR
export WDLDIR
export SDLDIR

OK:=echo -n

default all: prereq import buildtree buildkern buildlicense buildw32

#XXX move this into configure
prereq: Makefile
	# check DL paths always, since these are more volatile than build user and tools
	# we enforce these directories to exist to be sure that strong permissions are
	# preserved during the switch to build user work of the build process.
	@if [ ! -d $(DLDIR) ]; then \
		echo "Error: invalid DLDIR path given for vm kernel package download directory."; \
		echo "directory \"$(DLDIR)\" does not exist."; \
		exit 1; \
	fi;
	@if [ ! -d $(WDLDIR) ]; then \
		echo "Error: invalid WDLDIR path given for win32 package download directory."; \
		echo "directory \"$(WDLDIR)\" does not exist."; \
		exit 1; \
	fi;
	@if [ ! -d $(SDLDIR) ]; then \
		echo "Error: invalid SDLDIR path given for local upstream repository mirror directory."; \
		echo "directory \"$(SDLDIR)\" does not exist."; \
		exit 1; \
	fi;
	@if [ ! -f .build_prereqs_verified ]; then \
		echo "Verifying build prerequisites ..." >&2; \
		NOFOUND=""; \
		REQS="make gcc g++ gawk bison flex unzip bzip2 patch perl wget tar svn git autoconf mkisofs md5sum cut"; \
		for REQ in $$REQS; do \
			which $$REQ >/dev/null 2>&1; \
			if (( $$? != 0 )); then \
				export NOFOUND="$$REQ $$NOFOUND"; \
			fi; \
		done; \
		if [ "$$NOFOUND" != "" ]; then \
			echo "ERROR: Unable to locate the following mandatory applications: $$NOFOUND" >&2; \
			exit 1; \
		fi; \
		touch .test >/dev/null 2>&1 ; \
		if (( $$? != 0 )); then \
			echo "ERROR: Current build directory does not appear writable." >&2; \
			exit 1; \
		fi; \
		chown $(BUSER):$(BGROUP) .test >/dev/null 2>&1; \
		if (( $$? != 0 )); then \
			echo "ERROR: Build user $(BUSER) and group $(BGROUP) do not appear valid." >&2; \
			echo "       Please verify the settings you configured and set BUSER/BGROUP accordingly." >&2; \
			exit 1; \
		fi; \
		rm -f .test >/dev/null 2>&1; \
		touch .build_prereqs_verified; \
	fi
	@chown $(BUSER):$(BGROUP) $(DLDIR) || $(OK)
	@chown $(BUSER):$(BGROUP) $(WDLDIR) || $(OK)
	@chown $(BUSER):$(BGROUP) $(SDLDIR) || $(OK)

precache: prereq
	@echo "Attempting pre-cache of kamikaze packages ..."
	@su $(BUSER) -c "$(MAKE) IN_KAMIKAZE_PRECACHE=1 downloads"
	@echo "Attempting pre-cache of win32 build packages ..."
	@su $(BUSER) -c "$(MAKE) IN_WIN32_PRECACHE=1 DLCMD=build/win32/scripts/download.sh downloads"

import: prereq $(PRECACHE_OPT)
	@if [ ! -d $(SDLDIR)/kamikaze ]; then \
		echo "Mirroring local OpenWRT tree in $(SDLDIR) ..." >&2; \
		cd $(SDLDIR); \
		svn co svn://svn.openwrt.org/openwrt/trunk/ kamikaze ; \
		if (( $$? != 0 )); then \
			echo "ERROR: Unable to download a copy of the OpenWRT subversion tree." >&2; \
			rm -rf kamikaze; \
			exit 1; \
		fi; \
	else \
		echo "Updating local OpenWRT tree at $(SDLDIR)/kamikaze/ ..." >&2; \
		cd $(SDLDIR)/kamikaze/; \
		svn up; \
	fi 

buildtree: import
	@if [ ! -d build/kamikaze/$(TGTNAME) ]; then \
		echo "Creating Tor VM build tree ..."; \
		cd build/kamikaze; \
		svn export -r$(CWRTVER) $(SDLDIR)/kamikaze $(TGTNAME); \
		if (( $$? != 0 )); then \
			echo "ERROR: Unable to export working copy of local OpenWRT tree." >&2; \
			rm -rf $(TGTNAME); \
			exit 1; \
		fi; \
		cd $(TGTNAME); \
		for PFILE in $$(ls ../patches/); do \
			patch -p1 < ../patches/$$PFILE; \
			if (( $$? != 0 )); then \
				echo "ERROR: Unable to apply patch $$PFILE." >&2; \
				cd ..; \
				rm -rf $(TGTNAME); \
				exit 1; \
			fi; \
		done; \
		if [ -d $(DLDIR) ]; then \
			ln -s $(DLDIR) ./dl; \
		fi; \
		chown -R $(BUSER):$(BGROUP) . ; \
	fi

buildkern: buildtree
	@cd build/kamikaze/$(TGTNAME); \
	time su $(BUSER) -c "( $(MAKE) V=99 oldconfig && $(MAKE) world )"; \
	if (( $$? != 0 )); then \
		echo "ERROR: OpenWRT kernel build failed.  Check log for details." >&2; \
		exit 1; \
	fi

buildlicense: buildkern
	@echo "Generating License and other legal documentation archive ..."; \
	$(SHELL) build/kamikaze/scripts/genlicense.sh build/kamikaze/$(TGTNAME)/build_dir kernel-license-docs.tgz ; \
	if [ -f kernel-license-docs.tgz ]; then mv kernel-license-docs.tgz build/kamikaze/; fi;

# XXX: add instructions for automated win32 package builds with a vm using these hooks.
# see https://data.peertech.org/torbld
W32MK=WDLDIR=$(WDLDIR) all
ifneq (,$(BUILD_SCP_USER))
  W32MK:=BUILD_SCP_USER=$(BUILD_SCP_USER) BUILD_SCP_IDF=$(BUILD_SCP_IDF) BUILD_SCP_HOST=$(BUILD_SCP_HOST) BUILD_SCP_DIR=$(BUILD_SCP_DIR) $(W32MK)
endif
ifeq (TRUE,$(AUTO_SHUTDOWN))
  W32MK:=AUTO_SHUTDOWN=TRUE $(W32MK)
endif
ifeq (TRUE,$(DEBUG_NO_STRIP))
  W32MK:=DEBUG_NO_STRIP=TRUE $(W32MK)
endif
ifneq (,$(W32AUTO_BUILD_CMD))
  W32MK:=W32AUTO_BUILD_CMD="$(W32AUTO_BUILD_CMD)" $(W32MK)
endif

# NOTE: for now this is not dependent on buildkern/buildlicense though maybe it should be...
buildw32: 
	@cd build/win32; \
	chown -R $(BUSER):$(BGROUP) . ; \
	time su $(BUSER) -c "( $(MAKE) $(W32MK) )"; \
	if (( $$? != 0 )); then \
		echo "ERROR: Unable to create win32 build ISO image." >&2; \
		exit 1; \
	fi

.PHONY: clean prereq import buildkern buildlicense buildw32 precache


# end of our pre-caching package loop
endif
endif
