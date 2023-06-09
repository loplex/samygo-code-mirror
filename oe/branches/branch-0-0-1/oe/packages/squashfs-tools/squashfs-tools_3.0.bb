DESCRIPTION = "Squashfs is a highly compressed read-only filesystem for Linux."
SECTION = "base"
LICENSE = "GPLv2"
PR = "r1"

DEPENDS = "zlib"

CFLAGS += "-I."

SRC_URI = "${SOURCEFORGE_MIRROR}/squashfs/squashfs${@bb.data.getVar('PV',d,1).replace('r','-r')}.tar.gz \
	   file://Makefile-3.0 \
	   file://fix-return-ok.patch;patch=1 \
"

S = "${WORKDIR}/squashfs${@bb.data.getVar('PV',d,1).replace('r','-r')}/squashfs-tools"

prefix = ""

do_compile_prepend () {
	cp -vf ${WORKDIR}/Makefile-3.0 ${S}/Makefile
}

do_compile() {
	oe_runmake mksquashfs unsquashfs
}

do_install () {
	install -d ${D}${sbindir}
	install -m 0755 mksquashfs ${D}${sbindir}/
	install -m 0755 unsquashfs ${D}${sbindir}/
}
