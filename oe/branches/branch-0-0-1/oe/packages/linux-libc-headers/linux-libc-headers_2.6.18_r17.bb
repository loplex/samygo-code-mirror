SECTION = "devel"
DESCRIPTION = "Sanitized set of 2.6 kernel headers for the C library's use."
# This package is derived from the original linux-libc-headers at
#   http://ep09.pld-linux.org/~mmazur/linux-libc-headers/
# More specifically, llh-2.6.12.0 was patched up to 2.6.16-rc6 with 
# the official linux patches (where applicable) and then fixed up just
# enough to build glibc-2.4. BEWARE!
#
# license note from the linux-libc-headers package: 
#   Linux-libc-headers are derived from linux kernel headers. For license of a
#   particular header, check it's content, and if copyright notice isn't present,
#   standard linux kernel license applies. 
# since we assume GPL for linux i think we can also assume it here
LICENSE = "GPL"
DEFAULT_PREFERENCE = "-1"
DEFAULT_PREFERENCE_T-SPHAUSC = "1"
DEFAULT_PREFERENCE_T-RBYAUSC = "1"

INHIBIT_DEFAULT_DEPS = "1"
PR = "r17"

# LN46A750.zip & LN52A750.zip same md5sum
SRC_URI = "${SAMSUNG_OS_MIRROR}/LN52A750.zip"

S = "${WORKDIR}/linux"

do_unpack2() {
        tar -xvf ${WORKDIR}/linux_A1.tar -C ${WORKDIR}/
        rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz ${WORKDIR}/*.tar || true
}

addtask unpack2 before do_patch after do_unpack

do_configure () {
	case ${TARGET_ARCH} in
		alpha*)   ARCH=alpha ;;
		arm*)     ARCH=arm ;;
		cris*)    ARCH=cris ;;
		hppa*)    ARCH=parisc ;;
		i*86*)    ARCH=i386 ;;
		ia64*)    ARCH=ia64 ;;
		mips*)    ARCH=mips ;;
		m68k*)    ARCH=m68k ;;
		powerpc*) ARCH=ppc ;;
		s390*)    ARCH=s390 ;;
		sh*)      ARCH=sh ;;
		sparc64*) ARCH=sparc64 ;;
		sparc*)   ARCH=sparc ;;
		x86_64*)  ARCH=x86_64 ;;
	esac
	
	echo > .mvl_cross_compile
	echo ${ARCH} > .mvl_target_cpu
	oe_runmake oldconfig
	oe_runmake include/linux/version.h

	rm include/asm/arch-ssdtv || true
	rm include/asm || true
	
	cp -pPR "include/asm-$ARCH" "include/asm"
	if test "$ARCH" = "arm"; then
		cp -pPR ${WORKDIR}/linux/ssdtv_platform/include/asm-${ARCH}/arch-ssdtv include/asm/arch-ssdtv || true
		cp -pPR include/asm/arch-ssdtv include/asm/arch || true
	elif test "$ARCH" = "sh"; then
		cp -pPR include/asm/cpu-${TARGET_ARCH} include/asm/cpu || die "unable to create include/asm/cpu"
	fi
	echo "all:" > Makefile
	rm -f ".co*" "*"
	rm -rf "[^i]*" "init" "ipc"
}

do_stage () {
	install -d ${STAGING_INCDIR}
	rm -rf ${STAGING_INCDIR}/linux ${STAGING_INCDIR}/asm ${STAGING_INCDIR}/asm-generic
	cp -pfLR include/linux ${STAGING_INCDIR}/
	cp -pfLR include/asm ${STAGING_INCDIR}/
	cp -pfLR include/asm-generic ${STAGING_INCDIR}/
	rm -rf ${CROSS_DIR}/${TARGET_SYS}/include/linux
	rm -rf ${CROSS_DIR}/${TARGET_SYS}/include/asm
	rm -rf ${CROSS_DIR}/${TARGET_SYS}/include/asm-generic
	install -d ${CROSS_DIR}/${TARGET_SYS}/include
	cp -pfLR include/linux ${CROSS_DIR}/${TARGET_SYS}/include/
	cp -pfLR include/asm ${CROSS_DIR}/${TARGET_SYS}/include/
	cp -pfLR include/asm-generic ${CROSS_DIR}/${TARGET_SYS}/include/
}

do_install() {
	install -d ${D}${includedir}
	cp -pfLR include/linux ${D}${includedir}/
	cp -pfLR include/asm ${D}${includedir}/
	cp -pfLR include/asm-generic ${D}${includedir}/
}

