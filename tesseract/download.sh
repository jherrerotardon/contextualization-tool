#!/bin/bash

downloadDirectory=`dirname $(readlink -f $0)`
filename=`basename $0`

mv $0 /tmp/$filename
git clone https://github.com/tesseract-ocr/tesseract.git $downloadDirectory
mv /tmp/$filename $downloadDirectory/$filename
