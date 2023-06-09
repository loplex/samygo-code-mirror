DESCRIPTION = "mount UPnP server content as a linux filesystem"
HOMEPAGE = "http://djmount.sourceforge.net/"
LICENSE = "GPL"
DEPENDS = "libupnp fuse"
RDEPENDS = "fuse-utils fuse-module"
PR = "r06"

INITSCRIPT_NAME = "djmount"

inherit autotools

EXTRA_OECONF = "--with-external-libupnp --with-fuse-prefix=${STAGING_DIR}/${TARGET_SYS}"

SRC_URI = "${SOURCEFORGE_MIRROR}/djmount/djmount-0.71.tar.gz \
	file://fix-mkv-mime.patch;patch=1 \
#	file://disable-unneeded-buffer-check.patch;patch=1 \
	file://selp-buffsize.patch;patch=1 \
"

do_configure(){
	oe_runconf
}
