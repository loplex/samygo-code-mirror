HOMEPAGE = "http://fuse.sourceforge.net/sshfs.html"
DESCRIPTION = "This is a filesystem client based on the SSH File Transfer Protocol using FUSE."
MAINTAINER = "Koen Kooi <koen@linuxtogo.org>"

LICENSE_${PN} = "LGPL"

DEPENDS = "fakeroot-native fuse glib-2.0" 
RRECOMMENDS_${PN} = "fuse-module"

SRC_URI="${SOURCEFORGE_MIRROR}/fuse/${P}.tar.gz"

inherit autotools pkgconfig


