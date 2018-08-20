#!/bin/bash

selfDirectory=`dirname $(readlink -f $0)`
rootDirectory=$selfDirectory/..

git clone https://github.com/tesseract-ocr/tesseract.git $rootDirectory/tesseract

cp $rootDirectory/resources/languages/* $rootDirectory/tesseract/tessdata/
cp $rootDirectory/tesseract/src/api/tess_version.h.in $rootDirectory/tesseract/src/api/tess_version.h

