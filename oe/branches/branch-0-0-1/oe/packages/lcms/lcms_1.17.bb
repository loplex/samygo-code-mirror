DESCRIPTION = "Little cms is a small-footprint, speed optimized color management engine."
SECTION = "libs"
LICENSE = "LGPL"
SRC_URI = "${SOURCEFORGE_MIRROR}/lcms/lcms-${PV}.tar.gz"

PR = "r2"

inherit autotools

AUTOTOOLS_STAGE_PKGCONFIG = "1"

# broken lz linking in tifficc, we don't need it * Arris * 
EXTRA_OECONF_samygo = "--without-tiff"

do_stage() {
	autotools_stage_all
}
