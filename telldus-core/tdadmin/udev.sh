#!/bin/bash

if [ "${ID_VENDOR_ID}" == "1781" ]; then
	if [ "${ACTION}" == "add" ]; then
		@TDADMIN_PATH@tdadmin controller connect --pid=${ID_MODEL_ID} --vid=${ID_VENDOR_ID} --serial=${ID_SERIAL_SHORT}
	elif [ "${ACTION}" == "remove" ]; then
		@TDADMIN_PATH@tdadmin controller disconnect --pid=${ID_MODEL_ID} --vid=${ID_VENDOR_ID} --serial=${ID_SERIAL_SHORT}
	fi
fi
