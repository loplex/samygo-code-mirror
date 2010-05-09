require binutils.inc
LICENSE = "GPLv3"

INC_PR = "r1"
PR = "${INC_PR}.0"

SRC_URI = "http://www.samsung.com/global/opensource/files/32B650.zip \
	file://better_file_error.patch;patch=1 \
	file://ld_makefile.patch;patch=1 \
	file://detect-makeinfo.patch;patch=1"

do_unpack2() {
	tar --strip-components=2 -xvzf ${WORKDIR}/SELP.3.2.x-Chelsea.src.tgz SELP.3.2.x-Chelsea.src/Toolchain/binutils-2.17.50.tgz
	tar -xvzf binutils-2.17.50.tgz -C ${WORKDIR} || true
	rm binutils-2.17.50.tgz
	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz || true
}

addtask unpack2 before do_patch after do_unpack

