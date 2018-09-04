#!/bin/bash

selfDirectory=`dirname $(readlink -f $0)`
rootDirectory=$selfDirectory/..

if [ "$#" -ne 1 ]; then
	echo "Usage: setQtVersion <QtVersion>"
else
	cd $rootDirectory/bin
	sed -e "s/Qt\/.*\/?/Qt\/$1\//g" Makefile > Makefile_tmp
	rm Makefile && mv Makefile_tmp Makefile
	echo "Version changed succesfully."
fi