TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#Include paths of source files of tesseract.
INCLUDEPATH += $$PWD/tesseract/api
INCLUDEPATH += $$PWD/tesseract/ccmain
INCLUDEPATH += $$PWD/tesseract/ccutil
INCLUDEPATH += $$PWD/tesseract/ccstruct

#Include libs of leptonica and tesseract.
LIBS += -llept -ltesseract

SOURCES += \
        src/OcrCustom/main.cpp
