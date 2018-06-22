#!/bin/bash

selfDirectory=`dirname $(readlink -f $0)`

git clone https://github.com/tesseract-ocr/tesseract.git $selfDirectory/tesseract

cp $selfDirectory/resources/languages/* $selfDirectory/tesseract/tessdata/

