DESCRIPTION = "libusb is a library to provide userspace \
access to USB devices."
HOMEPAGE = "http://libusb.sf.net"
SECTION = "libs"
LICENSE = "LGPL"

PE = "1"
PR = "r0"

DEPENDS = "libusb1"

SRC_URI = "${SOURCEFORGE_MIRROR}/libusb/libusb-compat-${PV}.tar.bz2 \
          "

inherit autotools pkgconfig binconfig lib_package

PARALLEL_MAKE = ""
EXTRA_OECONF = "--disable-build-docs"

export CXXFLAGS += "-lstdc++ -I${STAGING_INCDIR}"

do_stage() {
    autotools_stage_all
    install -m 755 ${S}/libusb-config ${STAGING_BINDIR}
    # can we get rid of that? wouldn't a sed statement do as well?
    sed -i 's:\-L${libdir} :-L${STAGING_LIBDIR} :' ${STAGING_BINDIR}/libusb-config

    if [ "${STAGING_BINDIR}" != "${STAGING_BINDIR_CROSS}" ]; then
            install -d ${STAGING_BINDIR_CROSS}/
        mv ${STAGING_BINDIR}/libusb-config ${STAGING_BINDIR_CROSS}/libusb-config
    fi
}

do_stage_samygo() {
    autotools_stage_all
    install -m 755 ${S}/libusb-config ${STAGING_BINDIR}
    # can we get rid of that? wouldn't a sed statement do as well?
    sed -i 's:\-L${libdir} :-L${STAGING_LIBDIR} :' ${STAGING_BINDIR}/libusb-config
}

PACKAGES =+ "libusbpp"

FILES_libusbpp = "${libdir}/libusbpp*.so.*"
