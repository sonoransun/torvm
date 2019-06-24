# package caching (optional)
# for now just support a single cache source.
# sourceforge and some other upstreams simply too unreliable / block frequent dls
#
DLCMD=build/win32/scripts/download.sh
CACHE_URL_BASE=http://data.peertech.org/pkgcache


# all packages needed for kamikaze builds

GMP_F=gmp-4.2.4.tar.bz2
GMP_URL=ftp://ftp.gnu.org/pub/gnu/gmp/$(GMP_F)
GMP_SUM=5420b0e558a69a53b36f2b2c70a69f547e075d98366a585fc80cbbcce1efe368

MPFR_F=mpfr-2.3.2.tar.bz2
MPFR_URL=http://mpfr.loria.fr/mpfr-2.3.2/$(MPFR_F)
MPFR_SUM=18e078c996e182b7ceab32f2ab840e6a151b593e0cd5b83cb9d2960f212fba4c

M4_F=m4-1.4.5.tar.gz
M4_URL=ftp://ftp.gnu.org/pub/gnu/m4/$(M4_F)
M4_SUM=85427df4ad38b078f68c16f235f2cd2733256149854b7ee699f6d5a7f8d38610

AUTOCONF_F=autoconf-2.62.tar.bz2
AUTOCONF_URL=ftp://ftp.gnu.org/pub/gnu/autoconf/$(AUTOCONF_F)
AUTOCONF_SUM=42be7628e32fd3bebe07d684b11fb6e7e7920ef698fc4ccb3da6d77f91cefb96

AUTOMAKE_F=automake-1.9.6.tar.bz2
AUTOMAKE_URL=ftp://ftp.gnu.org/pub/gnu/automake/$(AUTOMAKE_F)
AUTOMAKE_SUM=8eccaa98e1863d10e4a5f861d8e2ec349a23e88cb12ad10f6b6f79022ad2bb8d

BISON_F=bison-2.3.tar.gz
BISON_URL=ftp://ftp.gnu.org/pub/gnu/bison/$(BISON_F)
BISON_SUM=52f78aa4761a74ceb7fdf770f3554dd84308c3b93c4255e3a5c17558ecda293e

PKGCFG_F=pkg-config-0.22.tar.gz
PKGCFG_URL=http://pkgconfig.freedesktop.org/releases/$(PKGCFG_F)
PKGCFG_SUM=7e0761b47d604847006e7c6caa9b9cf044530a516ff84395450edcfa3c2febe6

SED_F=sed-4.1.2.tar.gz
SED_URL=ftp://ftp.gnu.org/gnu/sed/$(SED_F)
SED_SUM=638e837ba765d5da0a30c98b57c2953cecea96827882f594612acace93ceeeab

IPKGUTIL_F=ipkg-utils-1.7.tar.gz
IPKGUTIL_URL=http://handhelds.org/packages/ipkg-utils/$(IPKGUTIL_F)
IPKGUTIL_SUM=813428c1aac04393ba64d0a23f9f820277357cbcdbdd391b9cf2bbf5257e8625

GENEXT2_F=genext2fs-1.4.1.tar.gz
GENEXT2_URL=http://downloads.sourceforge.net/genext2fs/$(GENEXT2_F)?big_mirror=1
GENEXT2_SUM=404dbbfa7a86a6c3de8225c8da254d026b17fd288e05cec4df2cc7e1f4feecfc

LZMAOLD_F=lzma-4.32.tar.bz2
LZMAOLD_URL=http://mirror2.openwrt.org/sources/$(LZMAOLD_F)
LZMAOLD_SUM=49053e4bb5e0646a841d250d9cb81f7714f5fff04a133216c4748163567acc3d

SQUASHFSOLD_F=squashfs3.0.tar.gz
SQUASHFSOLD_URL=http://downloads.sourceforge.net/squashfs/$(SQUASHFSOLD_F)?big_mirror=1
SQUASHFSOLD_SUM=39dbda43cf118536deb746c7730b468702d514a19f4cfab73b710e32908ddf20

LZMA_F=lzma-4.65.tar.bz2 
LZMA_URL=http://mirror2.openwrt.org/sources/$(LZMA_F)
LZMA_SUM=dcbdb5f4843eff638e4a5e8be0e2486a3c5483df73c70823618db8e66f609ec2

SQUASHFS_F=squashfs4.0.tar.gz
SQUASHFS_URL=http://downloads.sourceforge.net/squashfs/$(SQUASHFS_F)?big_mirror=1
SQUASHFS_SUM=18948edbe06bac2c4307eea99bfb962643e4b82e5b7edd541b4d743748e12e21

MTD_F=mtd_20050122.orig.tar.gz
MTD_URL=http://ftp.debian.org/debian/pool/main/m/mtd/$(MTD_F)
MTD_SUM=1ee1bb3338b9ec13037eedb03920b92d4d6336d936a581e0262d8687ddab9940

QUILT_F=quilt-0.47.tar.gz
QUILT_URL=http://download.savannah.gnu.org/releases/quilt/$(QUILT_F)
QUILT_SUM=100a6a7d5012d36590e607ebc87535f62318cfe7368f4cd44cacb94a2cf938a2

CCACHE_F=ccache-2.4.tar.gz
CCACHE_URL=http://samba.org/ftp/ccache/$(CCACHE_F)
CCACHE_SUM=435f862ca5168c346f5aa9e242174bbf19a5abcaeecfceeac2f194558827aaa0

BINUTILS_F=binutils-2.19.1.tar.bz2
BINUTILS_URL=ftp://ftp.gnu.org/gnu/binutils/$(BINUTILS_F)
BINUTILS_SUM=3e8225b4d7ace0a2039de752e11fd6922d3b89a7259a292c347391c4788739f6

GCC_F=gcc-4.4.0.tar.bz2
GCC_URL=ftp://ftp.gnu.org/gnu/gcc/gcc-4.4.0/$(GCC_F)
GCC_SUM=c5fe6f4c62ee7288765c3800ec9d21ad936bdcb5a04374cc09bd5a4232b836c9

LINUX26_F=linux-2.6.28.10.tar.bz2
LINUX26_URL=http://www.kernel.org/pub/linux/kernel/v2.6/$(LINUX26_F)
LINUX26_SUM=0b1849bb555e724c8e68726137853891d992034a1fb8b12ade67befba1f67983

BUSYBOX_F=busybox-1.11.3.tar.bz2
BUSYBOX_URL=http://www.busybox.net/downloads/$(BUSYBOX_F)
BUSYBOX_SUM=6d35fda668988c06f8c22e6e20fe6951ec83108169df6b8c0bad99872577c784

E2FSPROG_F=e2fsprogs-1.41.5.tar.gz
E2FSPROG_URL=http://downloads.sourceforge.net/e2fsprogs/$(E2FSPROG_F)?big_mirror=1
E2FSPROG_SUM=b3d7d0e1058a3740ddae83d47285bd9dce161eec9e299dde7996ed721da32198

LIBTOOL_F=libtool-1.5.24.tar.gz
LIBTOOL_URL=ftp://ftp.gnu.org/gnu/libtool/$(LIBTOOL_F)
LIBTOOL_SUM=1e54016a76e9704f11eccf9bb73e2faa0699f002b00b6630df82b8882ff2e5b2

IPTABLES_F=iptables-1.4.3.2.tar.bz2
IPTABLES_URL=http://www.netfilter.org/projects/iptables/files/$(IPTABLES_F)
IPTABLES_SUM=dec9b2248ba6824825011b73034bb43ca97d9c2d02e4024dc01549afd09ed3b1

IPROUTE2_F=iproute2-2.6.25.tar.bz2
IPROUTE2_URL=http://devresources.linux-foundation.org/dev/iproute2/download/$(IPROUTE2_F)
IPROUTE2_SUM=dc0e0f66c0928b5b4d4ae6bc833f3b21d0d4f5dbdaaf0711e42497bf50294512

LIBEVENT_F=libevent-1.4.9-stable.tar.gz
LIBEVENT_URL=http://www.monkey.org/~provos/$(LIBEVENT_F)
LIBEVENT_SUM=f5c72e07db5554e9c888dda6a5f7feb86f266e98ffa3f7cdb1e66b030d1d2cbf

LIBPCAP_F=libpcap-1.0.0.tar.gz
LIBPCAP_URL=http://www.tcpdump.org/release/$(LIBPCAP_F)
LIBPCAP_SUM=a214c4e1d7e22a758f66fe1d08f0ce41c3ba801a4c13dd1188e1e38288ac73c0

LIBUPNP_F=libupnp-1.6.6.tar.bz2
LIBUPNP_URL=http://downloads.sourceforge.net/pupnp/$(LIBUPNP_F)?big_mirror=1
LIBUPNP_SUM=58d7cabec2b21c80e28a4e5090bba94a849a8f02450e26c1b985318a36b0bbb3

ZLIB_F=zlib-1.2.3.tar.bz2
ZLIB_URL=http://www.zlib.net/$(ZLIB_F)
ZLIB_SUM=e3b9950851a19904d642c4dec518623382cf4d2ac24f70a76510c944330d28ca

OPENSSL_F=openssl-0.9.8k.tar.gz
OPENSSL_URL=http://www.openssl.org/source/$(OPENSSL_F)
OPENSSL_SUM=7e7cd4f3974199b729e6e3a0af08bd4279fde0370a1120c1a3b351ab090c6101

TOR_F=tor-0.2.2.5-alpha.tar.gz
TOR_URL=http://www.torproject.org/dist/$(TOR_F)
TOR_SUM=8b89fc25d09e00a7bd3d26cbdcda88092aaabed7124c12881b2d4fc8b6434dee


# iterative definition to retrieve all of the necessary packages.

ALL_NAMES=GMP MPFR M4 AUTOCONF AUTOMAKE BISON PKGCFG SED IPKGUTIL GENEXT2 LZMAOLD SQUASHFSOLD LZMA SQUASHFS MTD QUILT CCACHE BINUTILS GCC LINUX26 BUSYBOX E2FSPROG LIBTOOL IPTABLES IPROUTE2 LIBEVENT LIBPCAP LIBUPNP ZLIB OPENSSL TOR

define DOWNLOAD_exp
$(1):
	@if [ ! -e $(DLDIR)/$$($(1)_F) ]; then $(DLCMD) "$$($(1)_URL)" $$($(1)_SUM) "$(DLDIR)/$$($(1)_F)" || $(DLCMD) "$(CACHE_URL_BASE)/$$($(1)_F)" $$($(1)_SUM) "$(DLDIR)/$$($(1)_F)"; fi
endef

$(foreach pkgname,$(ALL_NAMES),$(eval $(call DOWNLOAD_exp,$(pkgname))))
     
downloads: $(ALL_NAMES)

.PHONY: downloads $(ALL_NAMES)

