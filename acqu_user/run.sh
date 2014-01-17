#!/bin/bash

LOGFILES="AcquRoot.log Analysis.log DataServer.log"

function print_logs() {
	for log in $LOGFILES; do
		echo ========= $log =========
		cat $log
		echo ""
	done
}

AcquRoot data/CB.Offline || {
	print_logs
	exit 1
}

exit 0


