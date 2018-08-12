## Contextualization Tool

## About

This project was developed by **Jorge Herrero Tardón** as **
Final Degree Project** in 2018.

It's a tool designed to contextualize strings with a capture to be sent to the translators. It also has an internal tool to recognize strings on a image using Optical Character Recognition (**OCR**) technology.

This application was developed in **C++** language using **Qt Framework**. It has a **GUI** mode and a **CLI** mode.

## Installing Contextualization Tool

### Build from source code

Pre-Requisites:

- A compiler for C and C++: GCC
- [Qt 5.10](https://www.qt.io/download) or above
- [Leptonica](http://www.leptonica.org/)

#### Leptonica

You also need to install [Leptonica](http://www.leptonica.org/). Ensure that the development headers for Leptonica are installed before compiling Tesseract.

Tesseract versions and the minimum version of Leptonica required:

**Tesseract** | **Leptonica** | **Ubuntu**
:-------------------: | :---------------------------------------: | :---------
4.00 | 1.74.2 | [Ubuntu 18.04](https://packages.ubuntu.com/bionic/tesseract-ocr)
3.05 | 1.74.0 | Must build from source
3.04 | 1.71 | [Ubuntu 16.04](http://packages.ubuntu.com/xenial/tesseract-ocr)
3.03 | 1.70 | [Ubuntu 14.04](http://packages.ubuntu.com/trusty/tesseract-ocr)
3.02 | 1.69 | Ubuntu 12.04
3.01 | 1.67 |

One option is to install the distro's Leptonica package:

```
sudo apt-get install libleptonica-dev
```

**but if you are using an oldish version of Linux, the Leptonica version may be too old, so you will need to build from source.**

The sources are at https://github.com/DanBloomberg/leptonica . The instructions for building are given in [Leptonica README](http://www.leptonica.org/source/README.html).

Note that if building Leptonica from source, you may need to ensure that /usr/local/lib is in your library path. This is a standard Linux bug, and the information at [Stackoverflow](http://stackoverflow.com/questions/4743233/is-usr-local-lib-searched-for-shared-libraries) is very helpful.

## Ubuntu

To compile source code is neccesary install:
```
sudo apt install libtesseract-dev
sudo apt-get install libgl1-mesa-dev
```

### Installing Contextualization Tool from Git

```
git clone https://jherrerotardon@bitbucket.org/jherrerotardon/contextualization-tool.git Contextualization-Tool
cd Contextualization-Tool
./install
```

The executable file will be in:
`./bin/ContextualizationTool`

## Windows

**Comming soon...**

## Running CLI Contextualization Tool

Basic command line help:
	`ContextualizationTool --help`

## Running GUI Contextualization Tool

Execute command without any arguments:
	`ContextualizationTool`

# References
***[Stackoverflow](https://github.com/tesseract-ocr/tesseract)***