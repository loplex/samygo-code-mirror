cmd_/opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/mmc/.install := perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a9/sources/kernel-headers/include/linux/mmc /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/mmc arm ioctl.h; perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a9/sources/kernel-headers/include/linux/mmc /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/mmc arm ; for F in ; do echo "\#include <asm-generic/$$F>" > /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/mmc/$$F; done; touch /opt/vd/arm-v7a9v3r1/arm-v7a9v3r1-linux-gnueabi/sys-root/./usr/include/linux/mmc/.install