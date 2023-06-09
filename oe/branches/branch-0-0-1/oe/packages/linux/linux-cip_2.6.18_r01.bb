DESCRIPTION = "Linux kernel for Samsung TV's"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
LICENSE = "GPL"
DEPENDS = "binutils-cross yes-native u-boot-mkimage-selp-native"

SRC_URI = "	${SAMSUNG_OS_MIRROR}/LE40B650T2P.zip \
		file://${MACHINE}-dotconfig \
		file://cip.Makefile.modpost \
		file://cip.Makefile.modinst \
		file://selp-fix_nonlinux_compile.patch;patch=1 \
"
SRC_URI_append_samygo += "file://selp-gadget.patch;patch=1 \ 
		file://selp-ralink-devlist.patch;patch=1;pnum=0 \ 
		file://selp-ralink-devlist_2.2.0.0.patch;patch=1;pnum=0 \
" 

S = "${WORKDIR}/linux/linux-r011"

KERNEL_VERSION_samygo = ${PV}
KERNEL_LOCALVERSION = "_SELP-ARM" 
inherit kernel

FILES_dvb-includes = "/usr/include/linux/dvb"
FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "arm.*-linux"
COMPATIBLE_MACHINE = "T-CH.CIPDEUC"

export OS = "Linux"
ARCH = "arm"

do_unpack2() {
	tar -xvzf ${WORKDIR}/linux.cip.open.tgz -C ${WORKDIR}/
	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz || true
 	# Branding
	perl -pi -e "s/0.994/0.994 \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt73/Module/rtmp_main.c
        perl -pi -e "s/1.002\(1.4.0.0\)/1.002\(1.4.0.0\) \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt2870/os/linux/2870_main_dev.c
}

addtask unpack2 before do_patch after do_unpack

do_configure_prepend() {
	echo ${CROSS_COMPILE} > .mvl_cross_compile
	echo ${TARGET_ARCH} > .mvl_target_cpu

#	./mkconfig.sh ${MACHINE_KERNEL_CONFIG_CMD_ARGS}  
#	make include/linux/version.h

####### Mod building/handling unknown at this time, so we fake some stuff	
	oe_machinstall -m 0644 ${WORKDIR}/${MACHINE}-dotconfig ${S}/.config
	oe_machinstall -m 0755 ${WORKDIR}/cip.Makefile.modpost ${S}/scripts/Makefile.modpost
	oe_machinstall -m 0755 ${WORKDIR}/cip.Makefile.modinst ${S}/scripts/Makefile.modinst
        oe_runmake defconfig
#	perl -pi -e "s/(CONFIG_MODULES)=y/# \1 is not set/" ${S}/.config
	rm scripts/mod/modpost && oe_runmake SUBDIRS=scripts/mod 
####### End of Modules stuff

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

