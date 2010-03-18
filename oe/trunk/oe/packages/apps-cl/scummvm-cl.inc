DESCRIPTION = "Virtual Machine for several classic graphical point-and-click adventure games"
HOMEPAGE = "http://www.scummvm.org"
SECTION = "games"
PRIORITY = "optional"
LICENSE = "GPL"
DEPENDS = "libsdl-cl libmad-cl flac-cl tremolo-cl fluidsynth-cl zlib"
PR = "r0"

SRC_URI = "${SOURCEFORGE_MIRROR}/scummvm/scummvm-${PV}.tar.bz2"

inherit autotools 

CCACHE = ""

PACKAGES = "${PN}"

EXTRA_OECONF = "--host=samsungtv \
		--backend=sdl \
		--disable-alsa \
		--prefix=${prefix} \
		--with-ogg-prefix=${STAGING_LIBDIR}/.. \
		--with-tremor-prefix=${STAGING_LIBDIR}/.. \
		--enable-release --disable-debug \
		"

EXTRA_OEMAKE = "MANDIR=${mandir}"

do_configure() {
	./configure ${EXTRA_OECONF}
	# Workaround, because some env variables aren't recognised correctly
	sed -i "s/AS := as/AS := ${AS}/" ${S}/config.mk
	sed -i "s/AR := ar cru/AR := ${AR} cru/" ${S}/config.mk
	sed -i "s/STRIP := strip/STRIP := ${STRIP}/" ${S}/config.mk
	sed -i "s/RANLIB := ranlib/RANLIB := ${RANLIB}/" ${S}/config.mk
}

do_install() {
	install -d ${D}scummvm
	install -m 0644 ${WORKDIR}/scummvm.so ${D}scummvm/libscummvm.so
}