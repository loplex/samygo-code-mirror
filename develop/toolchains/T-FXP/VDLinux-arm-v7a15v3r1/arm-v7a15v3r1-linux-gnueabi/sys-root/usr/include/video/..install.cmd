cmd_/opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/video/.install := perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a15/sources/kernel-headers/include/video /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/video arm edid.h sisfb.h uvesafb.h; perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a15/sources/kernel-headers/include/video /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/video arm ; for F in ; do echo "\#include <asm-generic/$$F>" > /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/video/$$F; done; touch /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/video/.install
