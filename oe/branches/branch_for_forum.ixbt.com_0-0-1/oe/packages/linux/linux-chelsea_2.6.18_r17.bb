DESCRIPTION = "Linux kernel for Samsung TV's"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
LICENSE = "GPL"
DEPENDS = "binutils-cross yes-native"

SRC_URI = "http://www.samsung.com/global/opensource/files/LN52A750.zip \
		http://www.samsung.com/global/opensource/files/LE40B650T2P.zip \
		file://selp-fix_nonlinux_compile.patch;patch=1 \
"
SRC_URI_append_selp += "file://selp-gadget.patch;patch=1" 
SRC_URI_append_selp += "file://selp-ralink-devlist.patch;patch=1;pnum=0" 
SRC_URI_append_selp += "file://selp-ralink-devlist_2.2.0.0.patch;patch=1;pnum=0" 

S = "${WORKDIR}/linux"

KERNEL_VERSION_selp = ${PV}
KERNEL_LOCALVERSION = "_SELP-ARM_V403_V6K" 
inherit kernel

FILES_dvb-includes = "/usr/include/linux/dvb"
FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "arm.*-linux"
COMPATIBLE_MACHINE = "T-SPHAUSC"

export OS = "Linux"
ARCH = "arm"
KERNEL_OUTPUT = "arch/arm/boot/Image"
KERNEL_OBJECT_SUFFIX = '.ko'

do_unpack2() {
        tar -vzf ${WORKDIR}/linux_A1.tar -C ${WORKDIR}/
	# Need Fix Arris
	tar -xvzf ${WORKDIR}/linux.cip.open.tgz -C ${WORKDIR}/ linux/linux-r011/drivers/rt73
	tar -xvzf ${WORKDIR}/linux.cip.open.tgz -C ${WORKDIR}/ linux/linux-r011/drivers/rt2870
	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz ${WORKDIR}/*.tar || true
 	# Branding
	perl -pi -e "s/0.994/0.994 \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt73/Module/rtmp_main.c
        perl -pi -e "s/1.002\(1.4.0.0\)/1.002\(1.4.0.0\) \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt2870/os/linux/2870_main_dev.c
}

addtask unpack2 before do_patch after do_unpack

do_configure_prepend() {
	echo ${CROSS_COMPILE} > .mvl_cross_compile
	echo ${TARGET_ARCH} > .mvl_target_cpu
	# ./mkconfig.sh ${MACHINE_KERNEL_CONFIG_CMD_ARGS}  
	# Need Fix Arris
	make defconfig
	make include/linux/version.h

	gcc_version=`${KERNEL_CC} -dumpversion`
                if [ "${gcc_version}" == "4.0.1" ] || [ "${gcc_version}" == "4.0.2" ]; then
                        die "tosa kernel wont work with gcc 4.0.x"
                fi
}

do_stage_prepend () {
	rm -f ${S}/include/asm-${TARGET_ARCH}/arch-ssdtv || true
	rm -f ${S}/include/asm-${TARGET_ARCH}/arch || true
	cp -avf ${WORKDIR}/linux/ssdtv_platform/include/asm-${TARGET_ARCH}/arch-ssdtv ${S}/include/asm-${TARGET_ARCH}/
}

do_stage_append () {
	install ${S}/.mvl_cross_compile ${STAGING_KERNEL_DIR}/ 
	install ${S}/.mvl_target_cpu ${STAGING_KERNEL_DIR}/
}

