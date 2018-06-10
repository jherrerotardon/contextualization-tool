#!/bin/bash

rm $0
git clone https://github.com/tesseract-ocr/tesseract.git $(dirname $(readlink -f $0))
