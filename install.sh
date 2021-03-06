#!/bin/bash

selfDirectory=`dirname $(readlink -f $0)`
tesseractDir=$selfDirectory/tesseract

# look if tesseract dir is empty 
if [ "$(ls -A $tesseractDir)" ]; then
     echo "$tesseractDir is not Empty"
else
	cd $selfDirectory &&
    ./scripts/loadTesseractFolder.sh &&
	make -C $selfDirectory/bin &&

	echo "Finished!!"
fi

