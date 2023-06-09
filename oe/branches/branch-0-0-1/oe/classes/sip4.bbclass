# Build Class for Sip based Python Bindings
# (C) Michael 'Mickey' Lauer <mickey@Vanille.de>
#
DEPENDS  =+ "sip4-native"
RDEPENDS += "python-sip4"

# default stuff, do not uncomment
# EXTRA_SIPTAGS = "-tWS_X11 -tQt_4_1_1"

sip4_do_generate() {
	if [ -z "${SIP_MODULES}" ]; then 
		MODULES="`ls sip/*mod.sip`"
	else
		MODULES="${SIP_MODULES}"
	fi

	if [ -z "$MODULES" ]; then
		die "SIP_MODULES not set and no modules found in $PWD"
        else
		oenote "using modules '${SIP_MODULES}' and tags '${EXTRA_SIPTAGS}'"
	fi

	if [ -z "${EXTRA_SIPTAGS}" ]; then
		die "EXTRA_SIPTAGS needs to be set!"
	else
		SIPTAGS="${EXTRA_SIPTAGS}"
	fi

	if [ ! -z "${SIP_FEATURES}" ]; then
		FEATURES="-z ${SIP_FEATURES}"
		oenote "sip feature file: ${SIP_FEATURES}"
	fi

	for module in $MODULES
	do
		install -d ${module}/
		echo "calling 'sip4 -I sip -I ${STAGING_SIPDIR} ${SIPTAGS} ${FEATURES} -c ${module} -b ${module}/${module}.pro.in sip/${module}/${module}mod.sip'"
		sip4 -I ${STAGING_SIPDIR} -I sip ${SIPTAGS} ${FEATURES} -c ${module} -b ${module}/${module}.sbf \
			sip/${module}/${module}mod.sip || die "Error calling sip on ${module}"
		cat ${module}/${module}.sbf 	| sed s,target,TARGET, \
						| sed s,sources,SOURCES, \
						| sed s,headers,HEADERS, \
						| sed s,"moc_HEADERS =","HEADERS +=", \
		>${module}/${module}.pro
		echo "TEMPLATE=lib" >>${module}/${module}.pro
		[ "${module}" = "qt" ] 		&& echo "" 		>>${module}/${module}.pro
		[ "${module}" = "qtcanvas" ] 	&& echo ""		>>${module}/${module}.pro
		[ "${module}" = "qttable" ] 	&& echo ""		>>${module}/${module}.pro
		[ "${module}" = "qwt" ] 	&& echo ""		>>${module}/${module}.pro
		[ "${module}" = "qtpe" ]        && echo ""		>>${module}/${module}.pro
		[ "${module}" = "qtpe" ]        && echo "LIBS+=-lqpe"	>>${module}/${module}.pro
		true
	done
}

EXPORT_FUNCTIONS do_generate

addtask generate after do_unpack do_patch before do_configure
