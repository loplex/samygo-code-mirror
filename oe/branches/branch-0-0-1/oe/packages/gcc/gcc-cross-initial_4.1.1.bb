require gcc-cross_${PV}.bb

DEPENDS = "virtual/${TARGET_PREFIX}binutils"
DEPENDS += "${@['virtual/${TARGET_PREFIX}libc-initial',''][bb.data.getVar('TARGET_ARCH', d, 1) in ['arm', 'armeb', 'mips', 'mipsel']]}"
PROVIDES = "virtual/${TARGET_PREFIX}gcc-initial"
PACKAGES = ""

# This is intended to be a -very- basic config
EXTRA_OECONF = "--with-local-prefix=${CROSS_DIR}/${TARGET_SYS} \
#		--with-newlib \
		--disable-shared \
		--disable-threads \
		--disable-multilib \
		--disable-__cxa_atexit \
		--disable-libmudflap \
		--disable-libssp \
		--enable-languages=c \
		--enable-target-optspace \
		--program-prefix=${TARGET_PREFIX} \
		${@get_gcc_fpu_setting(bb, d)}"

do_stage_prepend () {
	mkdir -p ${CROSS_DIR}/lib/gcc/${TARGET_SYS}/${BINV}
	ln -sf libgcc.a ${CROSS_DIR}/lib/gcc/${TARGET_SYS}/${BINV}/libgcc_eh.a
}

# Override the method from gcc-cross so we don't try to install libgcc
do_install () {
	oe_runmake 'DESTDIR=${D}' install
}
