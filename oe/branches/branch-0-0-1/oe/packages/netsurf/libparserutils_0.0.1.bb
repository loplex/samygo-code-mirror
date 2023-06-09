DESCRIPTION = "LibParserUtils is a library for building efficient parsers"
HOMEPAGE = "http://www.netsurf-browser.org/projects/libparserutils/"
SECTION = "libs"
PRIORITY = "optional"
LICENSE = "MIT"

SRC_URI = "http://www.netsurf-browser.org/projects/releases/libparserutils-${PV}-src.tar.gz"

inherit pkgconfig

CFLAGS_samygo =+ " -D_POSIX_C_SOURCE=1 -D_XOPEN_SOURCE -D_STRING_ARCH_unaligned -Wno-error"
CPPFLAGS_samygo =+ " -D_POSIX_C_SOURCE=1 -D_XOPEN_SOURCE -D_STRING_ARCH_unaligned -Wno-error"

EXTRA_OEMAKE = "CURDIR=${S} DESTDIR=${D} PREFIX=${prefix} BUILDDIR=build-OE"

# NOTE: we're using default buildmode here, which results in building only
# static libraries (.a) Not a problem as libparserutils is only used by Netsurf
# at the moment

do_stage () {
        oe_libinstall -a -C build-OE/ libparserutils ${STAGING_LIBDIR}

	install -d ${STAGING_INCDIR}/parserutils
	install -d ${STAGING_INCDIR}/parserutils/charset
	install -d ${STAGING_INCDIR}/parserutils/input
	install -d ${STAGING_INCDIR}/parserutils/utils
	install -m 0644 include/parserutils/*.h ${STAGING_INCDIR}/parserutils
	install -m 0644 include/parserutils/charset/*.h \
		${STAGING_INCDIR}/parserutils/charset
	install -m 0644 include/parserutils/input/*.h \
		${STAGING_INCDIR}/parserutils/input
	install -m 0644 include/parserutils/utils/*.h \
		${STAGING_INCDIR}/parserutils/utils
}


do_install() {
	oe_runmake install
}

