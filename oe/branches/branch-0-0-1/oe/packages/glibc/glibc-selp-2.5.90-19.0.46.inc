require glibc.inc
PR = "${INC_PR}.1"

ARM_INSTRUCTION_SET = "arm"

PACKAGES_DYNAMIC = "libc6*"
RPROVIDES_${PN}-dev = "libc6-dev virtual-libc-dev"

# the -isystem in bitbake.conf screws up glibc do_stage
BUILD_CPPFLAGS = "-I${STAGING_INCDIR_NATIVE}"
TARGET_CPPFLAGS = "-I${STAGING_DIR_TARGET}${layout_includedir}"

FILESDIR = "${@os.path.dirname(bb.data.getVar('FILE',d,1))}/glibc-2.4"

GLIBC_ADDONS ?= "ports,nptl,libidn"

GLIBC_BROKEN_LOCALES = "sid_ET tr_TR mn_MN gez_ET gez_ER bn_BD te_IN"

#
# For now, we will skip building of a gcc package if it is a uclibc one
# and our build is not a uclibc one, and we skip a glibc one if our build
# is a uclibc build.
#
# See the note in gcc/gcc_3.4.0.oe
#

python __anonymous () {
    import bb, re
    uc_os = (re.match('.*uclibc$', bb.data.getVar('TARGET_OS', d, 1)) != None)
    if uc_os:
        raise bb.parse.SkipPackage("incompatible with target %s" %
                                   bb.data.getVar('TARGET_OS', d, 1))
}

RDEPENDS_${PN}-dev = "linux-libc-headers-dev"

SRC_URI = "\
   ${SAMSUNG_OS_MIRROR}/LN40C550.zip \
  file://generate-supported.mk \
"

# Build fails on sh3 and sh4 without additional patches
SRC_URI_append_sh3 = " file://no-z-defs.patch;patch=1"
SRC_URI_append_sh4 = " file://no-z-defs.patch;patch=1"

S = "${WORKDIR}/selp_glibc-${PV}"
B = "${WORKDIR}/build-${TARGET_SYS}"

EXTRA_OECONF = "\
  --enable-kernel=${OLDEST_KERNEL} \
  --without-cvs --disable-profile --disable-debug --without-gd \
  --enable-clocale=gnu \
  --enable-add-ons=${GLIBC_ADDONS} \
  --with-headers=${STAGING_INCDIR} \
  --without-selinux \
  ${GLIBC_EXTRA_OECONF} \
"

EXTRA_OECONF += "${@get_glibc_fpu_setting(bb, d)}"

do_unpack2() {
        tar -xvzf ${WORKDIR}/selp_glibc-${PV}.tgz -C ${WORKDIR}/
        rm -rf ${WORKDIR}/*[zr]
}

addtask unpack2 before do_patch after do_unpack

do_munge() {
	rm -rf ${S}/glibc-build-localedef
	rm -rf ${S}/localedef
}

addtask munge before do_patch after do_unpack2

do_configure () {
# override this function to avoid the autoconf/automake/aclocal/autoheader
# calls for now
# don't pass CPPFLAGS into configure, since it upsets the kernel-headers
# version check and doesn't really help with anything
	if [ -z "`which rpcgen`" ]; then
		echo "rpcgen not found.  Install glibc-devel."
		exit 1
	fi
	(cd ${S} && gnu-configize) || die "failure in running gnu-configize"
	CPPFLAGS="" oe_runconf
}

rpcsvc = "bootparam_prot.x nlm_prot.x rstat.x \
	  yppasswd.x klm_prot.x rex.x sm_inter.x mount.x \
	  rusers.x spray.x nfs_prot.x rquota.x key_prot.x"

do_compile () {
	# -Wl,-rpath-link <staging>/lib in LDFLAGS can cause breakage if another glibc is in staging
	unset LDFLAGS
	base_do_compile
	(
		cd ${S}/sunrpc/rpcsvc
		for r in ${rpcsvc}; do
			h=`echo $r|sed -e's,\.x$,.h,'`
			rpcgen -h $r -o $h || oewarn "unable to generate header for $r"
		done
	)
}

require glibc-stage.inc

require glibc-package.inc
