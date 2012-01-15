require gcc-configure-common.inc

USE_NLS = '${@base_conditional( "TARGET_OS", "linux-uclibc", "no", "", d )}'

EXTRA_OECONF_PATHS = "--with-sysroot=${STAGING_DIR_TARGET} \
                      --with-build-sysroot=${STAGING_DIR_TARGET} \
		      --with-build-time-tools=${STAGING_BINDIR_CROSS}"

do_configure_prepend () {
	export CC="${BUILD_CC}"
	export AR_FOR_TARGET="${TARGET_SYS}-ar"
	export AS_FOR_TARGET="${TARGET_SYS}-as"
	export RANLIB_FOR_TARGET="${TARGET_SYS}-ranlib"
	export LD_FOR_TARGET="${TARGET_SYS}-ld"
	export NM_FOR_TARGET="${TARGET_SYS}-nm"
	export OBJDUMP_FOR_TARGET="${TARGET_SYS}-objdump"
	export OBJCOPY_FOR_TARGET="${TARGET_SYS}-objcopy"
	export STRIP_FOR_TARGET="${TARGET_SYS}-strip"
	export CC_FOR_TARGET="${CCACHE} ${TARGET_SYS}-gcc ${TARGET_CC_ARCH}"
}

LIBGCCS_VAR = "-lgcc_s"
LIBGCCS_VAR_avr32 = ""