## Contextualization Tool

## About

This project was developed by **Jorge Herrero Tardón** as **Final Degree Project** in 2018.

It's a tool designed to contextualize strings with a capture to be sent to the translators. It also has an internal tool to recognize strings on a image using Optical Character Recognition (**OCR**) technology.

This application was developed in **C++** language using **Qt Framework**. It has a **GUI** mode and a **CLI** mode.

## Installing Contextualization Tool

### Build from source code

Pre-Requisites:

- A compiler for C and C++: GCC
- [Qt 5.6](https://www.qt.io/download) or above
- [Leptonica 1.74](http://www.leptonica.org/) or above
- [Tesseract OCR 4.0] (https://github.com/tesseract-ocr/master) or above

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

**But this option only is valid in Unbuntu 18 or above. In less versions of Ubuntu, as Ubuntu 16.04, you must to install leptonica manually (build from source).**

One fast option is exec script allocated in scripts folder name **installTesseractLeptonica.sh**. This script also install Tesseract OCR libraries.

The sources are at https://github.com/DanBloomberg/leptonica . The instructions for building are given in [Leptonica README](http://www.leptonica.org/source/README.html).

Note that if building Leptonica from source, you may need to ensure that /usr/local/lib is in your library path. This is a standard Linux bug, and the information at [Stackoverflow](http://stackoverflow.com/questions/4743233/is-usr-local-lib-searched-for-shared-libraries) is very helpful.

## Ubuntu

To compile source code is neccesary install:
```
sudo apt install libtesseract-dev
sudo apt-get install libgl1-mesa-dev
```
**Only valid in Ubuntu 18 or above.**

In older versions ob Ubuntu yo must execute scripts/installTesseractLeptonica.sh to build libraries from source.

### Installing Contextualization Tool from Git

To get project from repository, yo can clone this in your locar storage:

```
git clone https://jherrerotardon@bitbucket.org/jherrerotardon/contextualization-tool.git Contextualization-Tool
cd Contextualization-Tool
```

Before install tool, sure that you have set correct version of Qt.

To set Qt version to compile tool, exec script **setQtVersion.sh**. Example:

```
./scripts/setQtVersion.sh 5.11.0
```

Finally, yo can install the tool.

```
./install
```

The executable file will be in:
`./contextualization`
## Windows

**Comming soon...**

## Running CLI Contextualization Tool

Basic command line help:
	`ContextualizationTool --help`

## Running GUI Contextualization Tool

Execute command without any arguments:
	`ContextualizationTool`

# References
***[Tesseract OCR](https://github.com/tesseract-ocr/tesseract)***
