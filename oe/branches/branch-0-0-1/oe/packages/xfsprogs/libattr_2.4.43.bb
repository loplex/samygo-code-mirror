DESCRIPTION = "library functions to get attribute bits"
HOMEPAGE = "http://oss.sgi.com/projects/xfs"
LICENSE = "GPL"
SECTION = "base"
PR = "r0"
# DEPENDS = "gettext"

SRC_URI = "ftp://oss.sgi.com/projects/xfs/cmd_tars/attr_${PV}-1.tar.gz"
S = "${WORKDIR}/attr-${PV}"

PARALLEL_MAKE = ""
inherit autotools
EXTRA_OECONF = "--enable-gettext=no --enable-shared=yes"

do_configure () {
	export LIBTOOL="${STAGING_BINDIR}/${HOST_SYS}-libtool"
	export DEBUG="-DNDEBUG"
	oe_runconf
}

do_install () {
	export DIST_ROOT=${D}
	oe_runmake install || true
	oe_runmake install-dev || true
	oe_libinstall -a libattr ${D}${libdir}
	oe_libinstall -a libattr ${STAGING_LIBDIR}
}

do_stage_append () {
#        autotools_stage_includes
#        oe_libinstall -so -C .libs libpopt ${STAGING_LIBDIR}/
	mkdir -p ${STAGING_INCDIR}/attr
	install -m 644 include/*.h ${STAGING_INCDIR}/attr/
}

