require linux-chelsea-common_2.6.18.inc

SRC_URI += "file://defconfig_samsung \
"

KERNEL_IMAGETYPE_LIST = "zImage"

do_configure_prepend() {
	echo ${CROSS_COMPILE} > .mvl_cross_compile
	echo ${TARGET_ARCH} > .mvl_target_cpu
	cp ${WORKDIR}/defconfig_samsung .config
	make include/linux/version.h
}

