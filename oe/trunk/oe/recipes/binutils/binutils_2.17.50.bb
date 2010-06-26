require binutils.inc
LICENSE = "GPLv2"

INC_PR = "r1"
PR = "${INC_PR}.0"

S = "${WORKDIR}/git"

SRC_URI = "${SOURCEFORGE_MIRROR}/project/samygo/SamyGO%20OE/sources/git_sourceware.org.git.binutils.git_138bc74f170158c175024317a9ccee37aa7cf617.tar.bz2 \
	file://better_file_error.patch;patch=1 \
	file://ld_makefile.patch;patch=1 \
	file://detect-makeinfo.patch;patch=1"

SRC_URI[md5sum] = "82c8d96b364a6b0ca343383ad53a8aea"
SRC_URI[sha256sum] = "6b6c1f02c9f1e33cfbaf755d418a146b06c1ce6a84d6c655e530ba70f061812e"
