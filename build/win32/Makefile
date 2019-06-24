# Copyright (C) 2008-2009  The Tor Project, Inc.
# See LICENSE file for rights and terms.

SHELL=/bin/bash
export SHELL

ifeq (,$(DLCMD))
DLCMD=scripts/download.sh
endif

ifeq (,$(ISODIR))
	ISODIR=iso
endif
UNZIP:=unzip -qq
SVNCMD:=svn -q
GITCMD:=git 
LOCALGITOPT:=--local --bare -n

# various utilities are downloaded and packaged into an ISO image
# with an autorun.inf that launches a build of all the win32 parts.
# 
ZLIB_F=zlib-1.2.3-bin.zip
ZLIB_URL=http://downloads.sourceforge.net/gnuwin32/$(ZLIB_F)?big_mirror=1
ZLIB_SUM=b7140fd08aebb482e243457aded2c5ced8ecee1197ff78de2524766ef94d092b

GZIP_F=gzip-1.3.12-1-bin.zip
GZIP_URL=http://downloads.sourceforge.net/gnuwin32/$(GZIP_F)?big_mirror=1
GZIP_SUM=598bfc7de80c616dbbf8e53acbf8358c91c6d3a3e0ddec094c0e05c465d0661b

BZIP2_F=bzip2-1.0.5-bin.zip
BZIP2_URL=http://downloads.sourceforge.net/gnuwin32/$(BZIP2_F)?big_mirror=1
BZIP2_SUM=97137e4b9ac9e116d0877f9d266927fea91ad117b011f80cf034fea4ef7a534e

LIBARCH_F=libarchive-2.4.12-1-bin.zip
LIBARCH_URL=http://downloads.sourceforge.net/gnuwin32/$(LIBARCH_F)?big_mirror=1
LIBARCH_SUM=d52c630d394a21560e708edc5738d0e6551b6fb402cd24c0c454e154e2e2f354

BINUTILS_F=binutils-2.19.1-mingw32-bin.tar.gz
BINUTILS_URL=http://downloads.sourceforge.net/mingw/$(BINUTILS_F)?big_mirror=1
BINUTILS_SUM=42427d4e7adaacacddb4e58ec3a03f3724aa46db884c7a37635bec98ac80fb0c
#binutils-2.19.1-src.tar.gz

GCCORE_F=gcc-core-3.4.5-20060117-3.tar.gz
GCCORE_URL=http://downloads.sourceforge.net/mingw/$(GCCORE_F)?big_mirror=1
GCCORE_SUM=be73c9428664b891fd45124de5ca78286cae2a6515890838f1347f77d21852ca

GPP_F=gcc-g++-3.4.5-20060117-3.tar.gz
GPP_URL=http://downloads.sourceforge.net/mingw/$(GPP_F)?big_mirror=1
GPP_SUM=cb1815cea431d4b5d2809743c617253ff5c1578876ef01a476550a082e484f08

MINGWRUN_F=mingwrt-3.15.2-mingw32-dev.tar.gz
MINGWRUN_URL=http://downloads.sourceforge.net/mingw/$(MINGWRUN_F)?big_mirror=1
MINGWRUN_SUM=edbbbd5ded73aa42547512f546dad795179c100235ef4a7531c7b8ca612da214
#mingwrt-3.15.2-mingw32-src.tar.gz

MINGWUTIL_F=mingw-utils-0.3.tar.gz
MINGWUTIL_URL=http://downloads.sourceforge.net/mingw/$(MINGWUTIL_F)?big_mirror=1
MINGWUTIL_SUM=3f9d8b6e69662381f403305a18dcccba729476371eed4f0f0ebf58a9bb1afaa8
#mingw-utils-0.3-src.tar.gz

MAKE_F=mingw32-make-3.81-20080326-3.tar.gz
MAKE_URL=http://downloads.sourceforge.net/mingw/$(MAKE_F)?big_mirror=1
MAKE_SUM=7d42d19c8e3e36bd7fca1cfd3c758589a929eb6d953056b64a6c53091cfeb70e
#mingw32-make-3.81-20080326-src.tar.gz

W32API_F=w32api-3.13-mingw32-dev.tar.gz
W32API_URL=http://downloads.sourceforge.net/mingw/$(W32API_F)?big_mirror=1
W32API_SUM=7dbf01a06a0e21bd405fc397789bdbfdd21e69acb59383568a3d96ea1b1455a6
#w32api-3.13-mingw32-src.tar.gz

#MSYS_F=MSYS-1.0.11-20071204.tar.bz2
#MSYS_URL=http://downloads.sourceforge.net/mingw/$(MSYS_F)?big_mirror=1
#MSYS_SUM=9a1207b04ffaf8880c58de643609e4deed3e1563

MSYSCORE_F=msysCORE-1.0.11-rc-1-bin.tar.gz
MSYSCORE_URL=http://downloads.sourceforge.net/mingw/$(MSYSCORE_F)?big_mirror=1
MSYSCORE_SUM=3b8e9609b665cc50a8fffc4a0be9fb6673bdeb33fd63a88410851970edd5e6bf
#msysCORE-1.0.11-rc-1-src.tar.gz

LIBTOOL_F=libtool1.5-1.5.25a-1-bin.tar.bz2
LIBTOOL_URL=http://downloads.sourceforge.net/mingw/$(LIBTOOL_F)?big_mirror=1
LIBTOOL_SUM=76944bf9793dd54999ee6f53fd9d592a9f62f7c833d1e006223a3fffb6f01ff7
#libtool1.5-1.5.25a-1-src.tar.bz2

MINIRES_F=minires-1.01-1-MSYS-1.0.11-1.tar.bz2
MINIRES_URL=http://downloads.sourceforge.net/mingw/$(MINIRES_F)?big_mirror=1
MINIRES_SUM=9578f4fa39f7c8488ce94fe6618a32679c80b5efd867412c7793e91aa1735f26
#minires-1.01-1-MSYS-1.0.11-1-src.tar.bz2

MSYSZLIB_F=zlib-1.2.3-MSYS-1.0.11-1.tar.bz2
MSYSZLIB_URL=http://downloads.sourceforge.net/mingw/$(MSYSZLIB_F)?big_mirror=1
MSYSZLIB_SUM=f57e54c913b72ed8a42cb0c819863beda5af5476a7684283d9c567dfdb5599d2
#zlib-1.2.3-MSYS-1.0.11-1-src.tar.bz2

MSYSCRYPT_F=crypt-1.1-1-MSYS-1.0.11-1.tar.bz2
MSYSCRYPT_URL=http://downloads.sourceforge.net/mingw/$(MSYSCRYPT_F)?big_mirror=1
MSYSCRYPT_SUM=c2e3dc67245279536b33fe1de4a799335e0014a5e3f97078c2f9d40ad5a3d8fc
#crypt-1.1-1-MSYS-1.0.11-1-src.tar.bz2

MSYSSSL_F=openssl-0.9.8g-1-MSYS-1.0.11-2-dll098.tar.gz
MSYSSSL_URL=http://downloads.sourceforge.net/mingw/$(SSL_F)?big_mirror=1
MSYSSSL_SUM=ca567528d72f77466e5d715be9340f524436ee42fe637e8ac5df69c9d5320208
#openssl-0.9.8g-1-MSYS-1.0.11-2-src.tar.gz

MSYSSSH_F=openssh-4.7p1-MSYS-1.0.11-1-bin.tar.gz
MSYSSSH_URL=http://downloads.sourceforge.net/mingw/$(SSH_F)?big_mirror=1
MSYSSSH_SUM=dcd8b3309157e3e8814a7083e94bf9e5f95102e55c9c7737dcd9b8247feb16d6
#openssh-4.7p1-MSYS-1.0.11-1-src.tar.gz

ACONF_F=autoconf2.5-2.61-1-bin.tar.bz2
ACONF_URL=http://downloads.sourceforge.net/mingw/$(ACONF_F)?big_mirror=1
ACONF_SUM=04793e5340f8e802115eaa4a1ee330704d20a51d400fe923939f6f00726fc974
#autoconf2.5-2.61-1-src.tar.bz2

AMAKE_F=automake1.9-1.9.6-2-bin.tar.bz2
AMAKE_URL=http://downloads.sourceforge.net/mingw/$(AMAKE_F)?big_mirror=1
AMAKE_SUM=7f6f70f9d6a34a92c794a3c2ae46bd847c451a2a400be3fe87677b0cabc7014f
#automake1.9-1.9.6-2-src.tar.bz2

PERL_F=perl-5.6.1-MSYS-1.0.11-1.tar.bz2
PERL_URL=http://downloads.sourceforge.net/mingw/$(PERL_F)?big_mirror=1
PERL_SUM=65dd8765a8f7b22d15193e56ed06c309c29a01b02d877a14e071cff1f46454c8
#perl-5.6.1-MSYS-1.0.11-1-src.tar.bz2

GNURX_VER=2.5.1
GNURX_F=mingw-libgnurx-$(GNURX_VER)-src.tar.gz
GNURX_URL=http://downloads.sourceforge.net/mingw/$(GNURX_F)?big_mirror=1
GNURX_SUM=7147b7f806ec3d007843b38e19f42a5b7c65894a57ffc297a76b0dcd5f675d76

ZLIBSRC_VER=1.2.3
ZLIBSRC_F=zlib-$(ZLIBSRC_VER).tar.gz
ZLIBSRC_URL=http://www.zlib.net/$(ZLIBSRC_F)
ZLIBSRC_SUM=1795c7d067a43174113fdf03447532f373e1c6c57c08d61d9e4e9be5e244b05e

PTHREADS_VER=2-8-0
PTHREADS_F=pthreads-w32-$(PTHREADS_VER)-release.tar.gz
PTHREADS_URL=ftp://sourceware.org/pub/pthreads-win32/$(PTHREADS_F)
PTHREADS_SUM=ff7a2874b04581ed344f7d3bb0548d7798891216eebcee830887b37c85ab48fd

LIBEVENT_VER=2.0.1-alpha
LIBEVENT_F=libevent-$(LIBEVENT_VER).tar.gz
LIBEVENT_URL=http://www.monkey.org/~provos/$(LIBEVENT_F)
LIBEVENT_SUM=7284eddec53bb731fb9c49aa6c6071b03476d98410d25b59810fbcc610d4a469

GROFF_VER=1.19.2
GROFF_F=groff-$(GROFF_VER).tar.gz
GROFF_URL=http://ftp.gnu.org/gnu/groff/$(GROFF_F)
GROFF_SUM=d864fbd0bf6dea24e4f71736da9902a1cae86d0a1a893b5fe17118927257ec3e

CMAKE_VER=2.6.2
CMAKE_F=cmake-$(CMAKE_VER).tar.gz
CMAKE_URL=http://www.cmake.org/files/v2.6/$(CMAKE_F)
CMAKE_SUM=b3f5a9dfa97fb82cb1b7d78a62d949f93c8d4317af36674f337d27066fa6b7e9

SSLSRC_VER=0.9.8k
SSLSRC_F=openssl-$(SSLSRC_VER).tar.gz
SSLSRC_URL=http://www.openssl.org/source/$(SSLSRC_F)
SSLSRC_SUM=7e7cd4f3974199b729e6e3a0af08bd4279fde0370a1120c1a3b351ab090c6101

PYTHON_VER=3.0.1
PYTHON_F=Python-$(PYTHON_VER).tar.bz2
PYTHON_URL=http://www.python.org/ftp/python/3.0.1/$(PYTHON_F)
PYTHON_SUM=91afb6ac16d3d22bc6bfbc80726dc85ede32bf838f660cc67016c7d0a7079add

PY2EXE_VER=0.6.9
PY2EXE_F=py2exe-$(PY2EXE_VER).zip
PY2EXE_TGZ=py2exe.tar.gz
PY2EXE_URL=http://downloads.sourceforge.net/py2exe/$(PY2EXE_F)?big_mirror=1
PY2EXE_SUM=6bd383312e7d33eef2e43a5f236f9445e4f3e0f6b16333c6f183ed445c44ddbd

WIX_VER=3.0.4805.0
WIX_F=wix$(WIX_VER)-binaries.zip
WIX_TGZ=wix.tar.gz
WIX_URL=http://downloads.sourceforge.net/wix/$(WIX_F)?big_mirror=1
WIX_SUM=ffbe664c97f5abad8aeb806bc9315ddcfc252696dfaa4847f72efe026eb226c5

WIXSRC_VER=3.0.4805.0
WIXSRC_F=wix$(WIXSRC_VER)-sources.zip
WIXSRC_TGZ=wixsrc.tar.gz
WIXSRC_URL=http://downloads.sourceforge.net/wix/$(WIXSRC_F)?big_mirror=1
WIXSRC_SUM=18175aa9265d13b9b3a1c24c8892de34fda5206a519db27007ea001a18034adc

NSIS_VER=2.42
NSIS_F=nsis-$(NSIS_VER).zip
NSIS_URL=http://downloads.sourceforge.net/nsis/$(NSIS_F)?big_mirror=1
NSIS_SUM=87368aac4531b6f04ba0d599be33d345a06c337b7114eaf01fbd733ed8e62d34
NSIS_TGZ=nsis-2.42.tar.gz

# 7zip is a weird one; use MSI to silently install if needed via install.bat helper
SEVNZIP_DL=true
SEVNZIP_INST=true
SEVNZIP_VER=464
SEVNZIP_F=7z464.msi
SEVNZIP_URL=http://downloads.sourceforge.net/sevenzip/$(SEVNZIP_F)?big_mirror=1
SEVNZIP_SUM=1e5b8909ccd3dc09977a1d95a7dfb239d7e30bba6d887d58773f6ddeebba9a05

SDL_VER=1.2.13
SDL_F=SDL-$(SDL_VER).tar.gz
SDL_URL=http://www.libsdl.org/release/$(SDL_F)
SDL_SUM=94f99df1d60f296b57f4740650a71b6425da654044ca30f8f0ce34934429e132

WPCAPSRC_VER=4_1_beta5
WPCAPSRC_F=WpcapSrc_$(WPCAPSRC_VER).zip
WPCAPSRC_URL=http://www.winpcap.org/install/bin/$(WPCAPSRC_F)
WPCAPSRC_SUM=9e147ff344141da0ac76e71c31be7a8036ccba6030adfe208f27a97b793a9299

QEMU_VER=0.10.5
QEMU_F=qemu-$(QEMU_VER).tar.gz
QEMU_URL=http://download.savannah.gnu.org/releases/qemu/$(QEMU_F)
QEMU_SUM=d54f268f5545dd182d96c1b09e79af58efaf2e51695efd1331d560a552d657c1

KQEMU_VER=1.4.0pre1
KQEMU_F=kqemu-$(KQEMU_VER).tar.gz
KQEMU_URL=http://www.qemu.org/$(KQEMU_F)
KQEMU_SUM=bac3e84eae1c867b06cd06ed2bcf563ba8304bd4e5a900880c2abe61b9aab491

OVPN_VER=2.1_rc15
OVPN_F=openvpn-$(OVPN_VER).tar.gz
OVPN_URL=http://openvpn.net/release/$(OVPN_F)
OVPN_SUM=66cfe347da6ab29ae39c80527714079cdb5699fdf453f7e031dd2ec0172d13a5

WINDDK_F=1830_usa_ddk.iso
WINDDK_URL=http://download.microsoft.com/download/9/0/f/90f019ac-8243-48d3-91cf-81fc4093ecfd/$(WINDDK_F)
WINDDK_SUM=7a27263e4c3c89dee113eba75dc9145bf7019b43a2c5e24bc9382dbde79f7f8b

QT_VER=4.5.1
QT_F=qt-all-opensource-src-$(QT_VER).tar.bz2
QT_URL=ftp://ftp.qtsoftware.com/qt/source/$(QT_F)
QT_SUM=fffd80292a3e5353e2a032167551dd56752624c7fa188df97a2ba19e0e33a295

#POLIPO_VER=20080907
#POLIPO_F=polipo-$(POLIPO_VER).tar.gz
#POLIPO_URL=http://www.pps.jussieu.fr/~jch/software/files/polipo/$(POLIPO_F)
#POLIPO_SUM=c4c8b6f4d32e6d17e71160f2d4683a3ac444c81de321eeb1394a5575d973a200

TORBUTTON_XPI_VER=1.2.2
TORBUTTON_XPI_F=torbutton-$(TORBUTTON_XPI_VER)-fx.xpi
TORBUTTON_XPI_URL=http://releases.mozilla.org/pub/mozilla.org/addons/2275/$(TORBUTTON_XPI_F)
TORBUTTON_XPI_SUM=d08fa9c1efc0203b5cf9c3fbc551dc158963821b45f1e9fe5763427332e00192

BITTORRENT_VER=3.4.2
BITTORRENT_F=bittorrent_$(BITTORRENT_VER).orig.tar.gz
# gah, sometimes it is pub/debian/... and others debian/... ???
BITTORRENT_URL=ftp://cdn.debian.net/debian/pool/main/b/bittorrent/$(BITTORRENT_F)
BITTORRENT_SUM=5bc3fc2372b0765d626a01aa60575b297b7d90629db019e885cfd053bd949fc5

BTPATCH_VER=3.4.2-11.1
BTPATCH_F=bittorrent_$(BTPATCH_VER).diff.gz
BTPATCH_URL=ftp://cdn.debian.net/debian/pool/main/b/bittorrent/$(BTPATCH_F)
BTPATCH_SUM=9dffc33c7c31daef9c6e63aaad4cef8b137d515e31b0685b3205efbdd3cb6c21

MARBLE_NAME=marble-latest
MARBLE_TGZ=$(MARBLE_NAME).tar.gz
MARBLE_SVN=https://svn.vidalia-project.net/svn/marble/
MARBLE_BR=trunk
MARBLE_VER=3516

VIDALIA_NAME=vidalia-latest
VIDALIA_TGZ=$(VIDALIA_NAME).tar.gz
VIDALIA_SVN=https://svn.vidalia-project.net/svn/vidalia/
VIDALIA_BR=trunk
VIDALIA_VER=3730

TORSRC_NAME=tor-latest
TORSRC_TGZ=$(TORSRC_NAME).tar.gz
TORSRC_GIT=git://git.torproject.org/git/tor
TORSRC_VER=d407d81a447f7ff7d53ac838703574c89acbc732

TORBUTTON_NAME=torbutton-latest
TORBUTTON_TGZ=$(TORBUTTON_NAME).tar.gz
TORBUTTON_SVN=https://tor-svn.freehaven.net/svn/torbutton/
TORBUTTON_BR=trunk
TORBUTTON_VER=20331

POLIPOGIT_NAME=polipo-latest
POLIPOGIT_TGZ=$(POLIPOGIT_NAME).tar.gz
POLIPOGIT_GIT=git://git.torproject.org/git/polipo
POLIPOGIT_VER=12f9f0d353322b6e0194722e5284666f8158e11a

PYCRYPTO_NAME=pycrypto-latest
PYCRYPTO_TGZ=$(PYCRYPTO_NAME).tar.gz
PYCRYPTO_GIT=git://git.pycrypto.org:9419/crypto/pycrypto-2.x.git
PYCRYPTO_VER=812e01736ca936124daa36d15a4159e92a43b9db

THANDY_NAME=thandy-latest
THANDY_TGZ=$(THANDY_NAME).tar.gz
THANDY_GIT=git://git.torproject.org/git/thandy
THANDY_VER=88e9bef040e05e0d94846577cd1c0a3d8f9a0653

# XXX: tmp equivalent of exit 0 when chaining cmds
# buildscripts need some dire clean up...
OK:=echo -n

CACHE_URL_BASE=http://data.peertech.org/pkgcache

REPONAMES=PYCRYPTO THANDY POLIPOGIT TORBUTTON TORSRC VIDALIA MARBLE
TOOLSPKGNAMES=ZLIB GZIP BZIP2 LIBARCH BINUTILS GCCORE GPP MINGWRUN MINGWUTIL MAKE W32API MSYSCORE LIBTOOL MINIRES MSYSZLIB MSYSCRYPT MSYSSSL MSYSSSH AMAKE ACONF PERL NSIS WIX WIXSRC TORBUTTON_XPI
SRCPKGNAMES=ZLIBSRC PTHREADS LIBEVENT CMAKE QT SSLSRC GROFF PYTHON PY2EXE GNURX SDL WPCAPSRC QEMU KQEMU OVPN BITTORRENT BTPATCH
ALLPKGNAMES=$(TOOLSPKGNAMES) $(SRCPKGNAMES)

# adjust if we're dealing with non source, zip or tar archives for a package
SEVNZIP_OPT_DL=
SEVNZIP_OPT_PKG=
ifeq (true,$(SEVNZIP_DL))
  ALLPKGNAMES+=SEVNZIP
  ifeq (true,$(SEVNZIP_INST))
    SEVNZIP_OPT_PKG=$(SEVNZIP_F)
  endif
endif

ALLPKGFILES:=
define DOWNLOAD_exp
ALLPKGFILES:=$(ALLPKGFILES) $$($(1)_F)
$(1):   
	@if [ ! -e $(WDLDIR)/$$($(1)_F) ]; then $(DLCMD) "$$($(1)_URL)" $$($(1)_SUM) "$(WDLDIR)/$$($(1)_F)" || $(DLCMD) "$(CACHE_URL_BASE)/$$($(1)_F)" $$($(1)_SUM) "$(WDLDIR)/$$($(1)_F)"; fi
endef

$(foreach pkgname,$(ALLPKGNAMES),$(eval $(call DOWNLOAD_exp,$(pkgname))))

PKGENVFILE=pkgenv.sh
PKGENVSETUP:=echo
define PKGENV_exp
PKGENVSETUP:=$(PKGENVSETUP) ; echo export $(1)_F=$$($(1)_F) ; echo export $(1)_VER=$$($(1)_VER)
endef

$(foreach pkgname,$(SRCPKGNAMES),$(eval $(call PKGENV_exp,$(pkgname))))

define REPOENV_exp
PKGENVSETUP:=$(PKGENVSETUP) ; echo export $(1)_FILE=$$($(1)_TGZ) ; echo export $(1)_DIR=$$($(1)_NAME)
endef

$(foreach reponame,$(REPONAMES),$(eval $(call REPOENV_exp,$(reponame))))

# now for primary targets...
downloads: $(ALLPKGNAMES)

default all: downloads coderepodls extract buildiso runinstall

coderepodls:
	@( cd $(SDLDIR); if [ -d $(TORSRC_NAME) ]; then if [ ! -d $(TORSRC_NAME)/.git ]; then rm -rf $(TORSRC_NAME); fi; $(OK); fi )
	@( cd $(SDLDIR); if [ -d $(TORSRC_NAME) ]; then cd $(TORSRC_NAME); $(GITCMD) pull; $(OK); else $(GITCMD) clone $(TORSRC_GIT) $(TORSRC_NAME); fi )
	@( cd $(SDLDIR); if [ -d $(TORBUTTON_NAME) ]; then cd $(TORBUTTON_NAME); $(SVNCMD) up; $(OK); else $(SVNCMD) co "$(TORBUTTON_SVN)$(TORBUTTON_BR)" $(TORBUTTON_NAME) ; fi )
	@( cd $(SDLDIR); if [ -d $(VIDALIA_NAME) ]; then cd $(VIDALIA_NAME); $(SVNCMD) up; $(OK); else $(SVNCMD) co "$(VIDALIA_SVN)$(VIDALIA_BR)" $(VIDALIA_NAME) ; fi )
	@( cd $(SDLDIR); if [ -d $(MARBLE_NAME) ]; then cd $(MARBLE_NAME); $(SVNCMD) up; $(OK); else $(SVNCMD) co "$(MARBLE_SVN)$(MARBLE_BR)" $(MARBLE_NAME) ; fi )
	@( cd $(SDLDIR); if [ -d $(THANDY_NAME) ]; then cd $(THANDY_NAME); $(GITCMD) pull; $(OK); else $(GITCMD) clone $(THANDY_GIT) $(THANDY_NAME); fi ) 
	@( cd $(SDLDIR); if [ -d $(POLIPOGIT_NAME) ]; then cd $(POLIPOGIT_NAME); $(GITCMD) pull; $(OK); else $(GITCMD) clone $(POLIPOGIT_GIT) $(POLIPOGIT_NAME); fi ) 
	@( cd $(SDLDIR); if [ -d $(PYCRYPTO_NAME) ]; then cd $(PYCRYPTO_NAME); $(GITCMD) pull; $(OK); else $(GITCMD) clone $(PYCRYPTO_GIT) $(PYCRYPTO_NAME); fi ) 

extract: downloads coderepodls
	@if [ -d $(ISODIR) ]; then \
		echo "Removing previous ISO build directory."; \
		rm -rf $(ISODIR) >/dev/null 2>&1; \
	fi; \
	mkdir -p $(ISODIR)/dl/src; \
	mkdir $(ISODIR)/bin; \
	mkdir $(ISODIR)/add; \
	if [ -e ../kamikaze/kernel-license-docs.tgz ]; then \
		cp ../kamikaze/kernel-license-docs.tgz $(ISODIR)/add/; \
	fi; \
	if [ -e ../kamikaze/x86-vm/bin/openwrt-x86-vmlinuz ]; then \
		cp ../kamikaze/x86-vm/bin/openwrt-x86-vmlinuz $(ISODIR)/add/vmlinuz; \
	fi; \
	if [ -e "$(BUILD_SCP_IDF)" ]; then \
		mkdir $(ISODIR)/ssh; \
		cp "$(BUILD_SCP_IDF)" $(ISODIR)/ssh/; \
		echo "$(BUILD_SCP_USER)" > $(ISODIR)/ssh/user; \
		echo "$(BUILD_SCP_HOST)" > $(ISODIR)/ssh/host; \
		echo "$(BUILD_SCP_DIR)" > $(ISODIR)/ssh/dest; \
		echo "export BUILD_SCP_NOSUBDIR=$(BUILD_SCP_NOSUBDIR)" >> $(ISODIR)/bldopts; \
	fi; \
	if [[ "$(AUTO_SHUTDOWN)" != "" ]]; then \
		echo "export AUTO_SHUTDOWN=$(AUTO_SHUTDOWN)" >> $(ISODIR)/bldopts; \
	fi; \
	if [[ "$(DEBUG_NO_STRIP)" != "" ]]; then \
		echo "export DEBUG_NO_STRIP=$(DEBUG_NO_STRIP)" >> $(ISODIR)/bldopts; \
	fi; \
	echo "export SEVNZIP_INST=$(SEVNZIP_INST)" >> $(ISODIR)/bldopts; \
	echo "export SEVNZIP_PKG=$(SEVNZIP_OPT_PKG)" >> $(ISODIR)/bldopts; \
	$(SHELL) -c "$(PKGENVSETUP)" > $(ISODIR)/dl/src/$(PKGENVFILE) ; chmod +x $(ISODIR)/dl/src/$(PKGENVFILE); \
	echo "--- Copying packages for ISO image file system ..."; \
	cp files/autorun.inf files/install.bat $(ISODIR)/; \
	cp files/*.vbs $(ISODIR)/bin/; \
	cp files/uninstall.bat $(ISODIR)/add/; \
	cp files/hdd.img.gz $(ISODIR)/add/; \
	cp files/defpolipo.conf $(ISODIR)/add/; \
	cp files/prefs.js $(ISODIR)/add/; \
	gzip -d $(ISODIR)/add/hdd.img.gz; \
	cp files/fstab $(ISODIR)/bin/; \
	cp files/buildall.sh $(ISODIR)/dl/src/; \
	cp patches/* $(ISODIR)/dl/src/; \
	cp -a $(addprefix $(WDLDIR)/,$(ALLPKGFILES)) $(ISODIR)/dl/; \
	tar zxf $(WDLDIR)/$(MINGWRUN_F) bin; mv bin/* $(ISODIR)/bin/;rmdir bin; \
	$(UNZIP) $(WDLDIR)/$(LIBARCH_F) -d $(ISODIR)/ bin/bsdtar.exe bin/libarchive2.dll; \
	$(UNZIP) $(WDLDIR)/$(ZLIB_F) -d $(ISODIR)/; \
	$(UNZIP) $(WDLDIR)/$(GZIP_F) -d $(ISODIR)/; \
	$(UNZIP) $(WDLDIR)/$(BZIP2_F) -d $(ISODIR)/; \
	mv $(ISODIR)/dl/$(PTHREADS_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(QEMU_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(KQEMU_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(GROFF_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(CMAKE_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(SSLSRC_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(GNURX_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(SDL_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(OVPN_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(ZLIBSRC_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(LIBEVENT_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(PYTHON_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(QT_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(BITTORRENT_F) $(ISODIR)/dl/src/; \
	mv $(ISODIR)/dl/$(BTPATCH_F) $(ISODIR)/dl/src/; gzip -d $(ISODIR)/dl/src/$(BTPATCH_F); \
	echo "--- Repacking WinPCAP archive ..."; \
	$(UNZIP) $(WDLDIR)/$(WPCAPSRC_F); mv winpcap WpcapSrc_$(WPCAPSRC_VER); tar cf $(ISODIR)/dl/src/WpcapSrc_$(WPCAPSRC_VER).tar WpcapSrc_$(WPCAPSRC_VER); gzip -9 $(ISODIR)/dl/src/WpcapSrc_$(WPCAPSRC_VER).tar; rm -rf WpcapSrc_$(WPCAPSRC_VER) $(ISODIR)/dl/$(WPCAPSRC_F); \
	echo "--- Repacking Py2Exe archive ..."; \
	$(UNZIP) $(WDLDIR)/$(PY2EXE_F) -d tmp; mv tmp/py2exe* py2exe;rmdir tmp; tar zcf $(ISODIR)/dl/$(PY2EXE_TGZ) py2exe; rm -rf py2exe $(ISODIR)/dl/$(PY2EXE_F); \
	echo "--- Repacking NSIS archive ..."; \
	$(UNZIP) $(WDLDIR)/$(NSIS_F); tar zcf $(ISODIR)/dl/$(NSIS_TGZ) nsis-$(NSIS_VER); rm -rf nsis-$(NSIS_VER) $(ISODIR)/dl/$(NSIS_F); \
	echo "--- Repacking WiX archive ..."; \
	$(UNZIP) $(WDLDIR)/$(WIX_F) -d wix; tar zcf $(ISODIR)/dl/$(WIX_TGZ) wix; rm -rf wix $(ISODIR)/dl/$(WIX_F); \
	echo "--- Repacking WiX sources archive ..."; \
	$(UNZIP) $(WDLDIR)/$(WIXSRC_F) -d wixsrc; tar zcf $(ISODIR)/dl/$(WIXSRC_TGZ) wixsrc; rm -rf wixsrc $(ISODIR)/dl/$(WIXSRC_F); \
	echo "--- Exporting Vidalia version $(VIDALIA_VER) from local mirror ..."; \
	rm -rf $(VIDALIA_NAME); $(SVNCMD) export --revision $(VIDALIA_VER) $(SDLDIR)/$(VIDALIA_NAME) $(VIDALIA_NAME); \
	( echo "Updating geoip cache in Vidalia source tree ..."; cp -f files/build-geoip-cache.sh $(VIDALIA_NAME)/pkg/; cd $(VIDALIA_NAME)/pkg; ./build-geoip-cache.sh ; ls -l geoip-cache ); \
	tar zcf $(ISODIR)/dl/src/$(VIDALIA_TGZ) $(VIDALIA_NAME); rm -rf $(VIDALIA_NAME); \
	echo "--- Exporting Marble revision $(MARBLE_VER) from local mirror ..."; \
	rm -rf $(MARBLE_NAME); $(SVNCMD) export --revision $(MARBLE_VER) $(SDLDIR)/$(MARBLE_NAME) $(MARBLE_NAME) ; \
	tar zcf $(ISODIR)/dl/src/$(MARBLE_TGZ) $(MARBLE_NAME); rm -rf $(MARBLE_NAME); \
	echo "--- Exporting Polipo version $(POLIPOGIT_VER) from local mirror ..."; \
	rm -rf $(POLIPOGIT_NAME); $(GITCMD) clone $(LOCALGITOPTS) $(SDLDIR)/$(POLIPOGIT_NAME) $(POLIPOGIT_NAME) ; \
	( cd $(POLIPOGIT_NAME); $(GITCMD) checkout $(POLIPOGIT_VER); ); \
	tar zcf $(ISODIR)/dl/src/$(POLIPOGIT_TGZ) $(POLIPOGIT_NAME); rm -rf $(POLIPOGIT_NAME); \
	echo "--- Exporting Thandy version $(THANDY_VER) from local mirror ..."; \
	rm -rf $(THANDY_NAME); $(GITCMD) clone $(LOCALGITOPTS) $(SDLDIR)/$(THANDY_NAME) $(THANDY_NAME) ; \
	( cd $(THANDY_NAME); $(GITCMD) checkout $(THANDY_VER); ); \
	tar zcf $(ISODIR)/dl/src/$(THANDY_TGZ) $(THANDY_NAME); rm -rf $(THANDY_NAME); \
	echo "--- Exporting Tor version $(TORSRC_VER) from local mirror ..."; \
	rm -rf $(TORSRC_NAME); $(GITCMD) clone $(LOCALGITOPTS) $(SDLDIR)/$(TORSRC_NAME) $(TORSRC_NAME) ; \
	( cd $(TORSRC_NAME); $(GITCMD) checkout $(TORSRC_VER); ); \
	tar zcf $(ISODIR)/dl/src/$(TORSRC_TGZ) $(TORSRC_NAME); rm -rf $(TORSRC_NAME); \
	echo "--- Exporting PyCrypto version $(PYCRYPTO_VER) from local mirror ..."; \
	rm -rf $(PYCRYPTO_NAME); $(GITCMD) clone $(LOCALGITOPTS) $(SDLDIR)/$(PYCRYPTO_NAME) $(PYCRYPTO_NAME) ; \
	( cd $(PYCRYPTO_NAME); $(GITCMD) checkout $(PYCRYPTO_VER); ); \
	tar zcf $(ISODIR)/dl/src/$(PYCRYPTO_TGZ) $(PYCRYPTO_NAME); rm -rf $(PYCRYPTO_NAME); \
	if [ -f $(WDLDIR)/$(TORBUTTON_XPI_F) ]; then \
		echo "--- Using Mozilla.org TorButton $(TORBUTTON_XPI_F) ..."; \
		cp -f $(WDLDIR)/$(TORBUTTON_XPI_F) $(ISODIR)/dl/src/torbutton.xpi; \
	else \
		echo "--- Exporting and archiving Tor Button version $(TORBUTTON_VER) from local mirror ..."; \
		rm -rf $(TORBUTTON_NAME); $(SVNCMD) export --revision $(TORBUTTON_VER) $(SDLDIR)/$(TORBUTTON_NAME) $(TORBUTTON_NAME) ; \
		( cd $(TORBUTTON_NAME); $(SHELL) makexpi.sh ; ); \
		mv $(TORBUTTON_NAME)/pkg/*.xpi $(ISODIR)/dl/src/torbutton.xpi ; rm -rf $(TORBUTTON_NAME); \
	fi; \
	echo "--- Creating Tor GEOIP file Tor source ..."; \
	cat $(SDLDIR)/$(TORSRC_NAME)/src/config/geoip > $(ISODIR)/add/GEOIP.TXT; \
	mkisofs -hide-rr-moved -pad -l -o $(ISODIR)/add/geoip.iso -J $(ISODIR)/add/GEOIP.TXT ; \
	echo "--- Creating source archives from win32 tree ..."; \
	( cd src && tar zcf ../$(ISODIR)/dl/src/torvm-w32.tgz torvm-w32 ); \
	( cd src && tar zcf ../$(ISODIR)/dl/src/pkg.tgz pkg ); \
	touch $(ISODIR)/VMDEVISO.TXT

buildiso: extract
	@if [ -f w32build.iso ]; then \
		rm -f w32build.iso; \
	fi; \
	export ODIR=`pwd`; \
	( 	cd $(ISODIR); \
		mkisofs -hide-rr-moved -pad -l -o $$ODIR/w32build.iso -r -J .; \
	); if (( $$? != 0 )); then exit 1; fi

ifeq (,$(W32AUTO_BUILD_CMD))
  W32BLD=echo "Win32 build ISO image ready.  Please load the ISO into a Windows systems to start the build."
else
  W32BLD=now=`date`;echo "Starting win32 automated build at $$now";$(W32AUTO_BUILD_CMD);now=`date`;echo "Build done at $$now";true
endif

runinstall: buildiso
	@$(W32BLD)

.PHONY: downloads extract buildiso runinstall $(ALLPKGNAMES)
