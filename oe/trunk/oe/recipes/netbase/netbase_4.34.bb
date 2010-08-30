#LocalChange: updated to newer version
DESCRIPTION = "This package provides the necessary \
infrastructure for basic TCP/IP based networking."
SECTION = "base"
LICENSE = "GPL"
PR = "r42"

inherit update-rc.d

INITSCRIPT_NAME = "networking"
INITSCRIPT_PARAMS = "start 40 S . stop 40 0 6 1 ."
# On MNCI etc, start very late so that our own apps come up faster
INITSCRIPT_PARAMS_openmn = "start 85 1 2 3 4 5 . stop 85 0 6 1 ."
# On SlugOS (NSLU2) delay the stop until after network apps have exited
# Do not stop in single user - there's no way to sulogin!
INITSCRIPT_PARAMS_slugos = "start 42 S 0 6 ."

SRC_URI = "${DEBIAN_MIRROR}/main/n/netbase/netbase_${PV}.tar.gz \
  file://options \
  file://init \
  file://hosts \
  file://interfaces \
  file://if-pre-up.d \
  file://if-up.d \
  file://if-down.d \
  file://if-post-down.d \
"

SRC_URI[md5sum] = "19cb9cda8699218a4f21d988472cdad4"
SRC_URI[sha256sum] = "fe0ce52e6f1570da2347aacdde5d8cdee1dc9016e3880f9a67c5ef2a60bd016b"

do_install () {
	install -d ${D}${sysconfdir}/init.d
	install	-d ${D}${sbindir}
	install -d ${D}${mandir}/man8
	install -d ${D}${sysconfdir}/network/if-pre-up.d
	install -d ${D}${sysconfdir}/network/if-up.d
	install -d ${D}${sysconfdir}/network/if-down.d
	install -d ${D}${sysconfdir}/network/if-post-down.d

	for dir in if-pre-up.d if-up.d if-down.d if-post-down.d
	do
		for script in `ls -1 "${WORKDIR}/${dir}"`
		do		
			install -m 0755 "${WORKDIR}/${dir}/${script}" "${D}${sysconfdir}/network/${dir}"
		done
	done
		   
	install -m 0644 ${WORKDIR}/options ${D}${sysconfdir}/network/options
	install -m 0755 ${WORKDIR}/init ${D}${sysconfdir}/init.d/networking
	install -m 0644 ${WORKDIR}/hosts ${D}${sysconfdir}/hosts
	install -m 0644 etc-rpc ${D}${sysconfdir}/rpc
	install -m 0644 etc-protocols ${D}${sysconfdir}/protocols
	install -m 0644 etc-services ${D}${sysconfdir}/services
	install -m 0644 ${WORKDIR}/interfaces ${D}${sysconfdir}/network/interfaces
}

CONFFILES_${PN} = "${sysconfdir}/network/options ${sysconfdir}/hosts \
                   ${sysconfdir}/network/interfaces ${sysconfdir}/rpc \
                   ${sysconfdir}/protocols ${sysconfdir}/services"