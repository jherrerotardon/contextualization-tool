#!/bin/bash

## Scrip to compilling and install Leptonica and Tesseract libaries.

sudo apt-get install g++ && # or clang++ (presumably)
sudo apt-get install autoconf automake libtool &&
sudo apt-get install pkg-config &&
sudo apt-get install libpng-dev &&
sudo apt-get install libjpeg8-dev &&
sudo apt-get install libtiff5-dev &&
sudo apt-get install zlib1g-dev &&

## LEPTONICA
sudo apt install libpng-dev &&
sudo apt install libjpeg62-dev &&
sudo apt install libtiff5-dev &&
sudo apt install libwebp-dev &&
sudo apt install libopenjp2-7-dev &&
sudo apt install libgif-dev &&

wget http://www.leptonica.com/source/leptonica-1.75.3.tar.gz &&
tar -xzvf leptonica-1.75.3.tar.gz &&
mv leptonica-1.75.3 leptonica &&
cd leptonica &&

autoreconf -vi &&
./autobuild &&
./configure &&
LDFLAGS="-Wl,-rpath -Wl,/usr/local/lib" ./configure &&
sudo make install &&
cd .. &&
rm -Rf leptonica &&
rm leptonica-1.75.3.tar.gz &&

## TESSERACT
git clone https://github.com/tesseract-ocr/tesseract.git &&
cd tesseract &&
autoreconf -vi &&
./autogen.sh &&
./configure --enable-debug &&
LDFLAGS="-L/usr/local/lib" CFLAGS="-I/usr/local/include" make &&
sudo make install &&
sudo ldconfig &&
cd .. &&
rm -Rf tesseract &&