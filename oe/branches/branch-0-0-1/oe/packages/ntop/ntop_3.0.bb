LICENSE = "GPL"
DESCRIPTION = "ntop is network top"
SECTION = "console/network"
PRIORITY = "optional"
DEPENDS = "gdbm zlib libpcap libpng gd"
PR = "r1"

SRC_URI = "${SOURCEFORGE_MIRROR}/ntop/ntop-${PV}.tgz \
           file://${FILESDIR}/autotools.patch;patch=1 \
           file://${FILESDIR}/plugins.patch;patch=1"

inherit autotools

EXTRA_OECONF += " --without-ssl \
                  --with-gd-lib=${STAGING_LIBDIR} \
                  --with-gd-include=${STAGING_INCDIR} \
                  --with-zlib-lib=${STAGING_LIBDIR} \
                  --with-zlib-include=${STAGING_INCDIR} \
                  --with-pcap-lib=${STAGING_LIBDIR} \
                  --with-pcap-include=${STAGING_INCDIR} \
                  --with-libpng-lib=${STAGING_LIBDIR} \
                  --with-libpng-include=${STAGING_INCDIR} \
                  --with-gdbm-lib=${STAGING_LIBDIR} \
                  --with-gdbm-include=${STAGING_INCDIR} \
		--includedir=${STAGING_INCDIR} \
		--oldincludedir=${STAGING_INCDIR} \
"

FILES_ntop_append = " ${libdir}/ntop/plugins/*.so ${libdir}/libntop-*.so \
                       ${libdir}/libntopreport-*.so"
FILES_${PN}-dev = "${includedir} ${libdir}/libntop.so ${libdir}/libntopreport.so \
                   ${libdir}/*.a ${libdir}/libntopreport.a ${libdir}/*.la"
FILES_${PN}-dbg += "${libdir}/ntop/plugins/.debug"

do_configure_prepend () {
	if [ ! -e acinclude.m4 ]; then
		mv acinclude.m4.ntop acinclude.m4
	fi
	perl -pi -e "s/-I\/usr\/local\/include//g" configure.in
	perl -pi -e "s/-L\/usr\/local\/lib//g" configure.in
	rm -f libtool
	cp ${STAGING_BINDIR}/${TARGET_SYS}-libtool libtool
}
