require firmware.inc
require firmiszip.inc
# require firmware-cip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI = "http://downloadcenter.samsung.com/content/FM/200911/20091119185403937/${MACHINE}[__________PX__].exe"

# S = "${WORKDIR}
P_OFFSET = "3969566"
P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
