#! /bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin

firmware=`cat "/.info"`

case $firmware in
T-CHE6AUSC|T-CHE7AUSC|T-CHEAUSC|T-CHL5DAUC|T-CHL5DEUC|T-CHL7DAUC|T-CHL7DEUC|T-CHU7DAUC|T-CHU7DEUC)
	export LD_LIBRARY_PATH="/lib:/usr/lib:/Java/lib:/mtd_cmmlib/Comp_LIB:/mtd_cmmlib/InfoLink/lib:/mtd_cmmlib/GAME_LIB:/mtd_cmmlib/DRM_LIB:/mtd_cmmlib/YWidget_LIB"
	export MAPLE_DEFAULT_PATH=/mtd_cmmlib/InfoLink/lib
	export MAPLE_MANAGER_WIDGET_PATH=/mtd_down/widgets/manager
	export MAPLE_NORMAL_WIDGET_PATH=/mtd_down/widgets/normal
	export MAPLE_WIDGET_DATA_PATH=/mtd_down
	export MAPLE_WIDGET_INCLUDE_PATH=/mtd_down/widgets/inc
	export MAPLE_PLUGIN_DATA_PATH=/mtd_cmmlib/InfoLink/lib
	export KF_SLEEP_READ=-2
	echo 30000 > /mtd_rwarea/DelayValue.txt
	#touch /mtd_rwarea/DoPrintYahoo.txt
	export KF_NO_INTERACTIVE=1
	export KF_LOG=/dev/null
	cd /mtd_exe/

	if [ ! "`lsmod | grep samdrv`" ]; then
		insmod /mtd_drv/samdrv.ko
	fi

	if [ -f /.custom_exedsp_cmd ]; then
		CUSTOM_EXEDSP_CMD=`cat "/.custom_exedsp_cmd"`
	fi
	echo "*** Starting exeDSP ***"
	if [ "$CUSTOM_EXEDSP_CMD" ]; then
		$CUSTOM_EXEDSP_CMD
	else
		./exeDSP
	fi
	echo "*** Finished exeDSP ***"
	;;

*)
	echo "samsung-start.sh: Failed, unknown device!"
	;;
esac

