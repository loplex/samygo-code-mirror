require python.inc
#DEPENDS = "readline-native"
#DEPENDS = "openssl-native bzip2-full-native zlib-native readline-native"
PR = "${INC_PR}.2"

SRC_URI = "\
  http://www.python.org/ftp/python/${PV}/Python-${PV}.tar.bz2 \
  file://00-fix-bindir-libdir-for-cross.patch;patch=1 \
  file://04-default-is-optimized.patch;patch=1 \
  file://05-enable-ctypes-cross-build.patch;patch=1 \
  file://10-distutils-fix-swig-parameter.patch;patch=1 \
  file://11-distutils-never-modify-shebang-line.patch;patch=1 \
  file://12-distutils-prefix-is-inside-staging-area.patch;patch=1 \
  file://debug.patch;patch=1 \
  file://nohostlibs.patch;patch=1 \
"
S = "${WORKDIR}/Python-${PV}"

inherit native

EXTRA_OEMAKE = '\
  BUILD_SYS="" \
  HOST_SYS="" \
  LIBC="" \
'

do_stage_append() {
	install -m 0755 Parser/pgen ${STAGING_BINDIR}/pgen
}
