cmd_/opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/linux/netfilter/ipset/.install := perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a15/sources/kernel-headers/include/linux/netfilter/ipset /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/linux/netfilter/ipset arm ip_set.h ip_set_bitmap.h ip_set_hash.h ip_set_list.h; perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a15/sources/kernel-headers/include/linux/netfilter/ipset /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/linux/netfilter/ipset arm ; for F in ; do echo "\#include <asm-generic/$$F>" > /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/linux/netfilter/ipset/$$F; done; touch /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/linux/netfilter/ipset/.install
