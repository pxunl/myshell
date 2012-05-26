#!/bin/bash

#######################################
# uninstall mshell and command
# remove ils imv ipwd and son on
# 2012/05/22 16:54:36
######################################

MSHELL_PATH=$(whereis mshell | sed 's/[^\/]*//')

if [ -f ${MSHELL_PATH} ]
then 
	rm ${MSHELL_PATH}
	echo "remove ${MSHELL_PATH}"
else 
	echo "can't remove ${MSHELL_PATH}"
fi

TARGET_FILES="ils imv iwho ipwd icp irm iproc icat imkdir"
TARGET_DIR="/usr/bin"

for delete_file in ${TARGET_FILES}
do
	if [ -f ${TARGET_DIR}/${delete_file} ]
	then 
		rm ${TARGET_DIR}/${delete_file}
		echo "remove ${TARGET_DIR}/${delete_file}"
	else 
		echo "can't remove ${TARGET_DIR}/${delete_file}"
	fi
done
