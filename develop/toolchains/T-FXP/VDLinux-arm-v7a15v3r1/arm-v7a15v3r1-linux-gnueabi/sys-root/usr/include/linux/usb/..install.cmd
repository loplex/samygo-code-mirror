cmd_/opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/linux/usb/.install := perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a15/sources/kernel-headers/include/linux/usb /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/linux/usb arm audio.h cdc.h ch11.h ch9.h functionfs.h g_printer.h gadgetfs.h midi.h tmc.h; perl scripts/headers_install.pl /OpenSource/13_UExxF8xxx/binutils/arm-mips-src-20121018/build.arm.cortex-a15/sources/kernel-headers/include/linux/usb /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/linux/usb arm ; for F in ; do echo "\#include <asm-generic/$$F>" > /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/linux/usb/$$F; done; touch /opt/vd/arm-v7a15v3r1/arm-v7a15v3r1-linux-gnueabi/sys-root/./usr/include/linux/usb/.install
