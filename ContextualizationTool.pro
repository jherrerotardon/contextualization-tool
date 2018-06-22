QT += quick
QT += widgets
CONFIG += c++11

INCLUDEPATH += $$PWD/src

#Include paths of source files of tesseract.
INCLUDEPATH += $$PWD/tesseract/src/api
INCLUDEPATH += $$PWD/tesseract/src/ccmain
INCLUDEPATH += $$PWD/tesseract/src/ccutil
INCLUDEPATH += $$PWD/tesseract/src/ccstruct

#Include libs of leptonica and tesseract.
LIBS += -llept -ltesseract

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/tools/log.cpp \
    src/base/firmwarestring.cpp \
    src/base/contextualizationmodel.cpp \
    src/tools/utils.cpp \
    src/tools/ocr.cpp \
    src/gui/guicontextualizationcontroller.cpp \
    src/base/contextualizationcontrollerbase.cpp \
    src/console/consolecontextualizationcontroller.cpp

RESOURCES += \
    resources/views.qrc \
    resources/images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/tools/log.h \
    src/base/firmwarestring.h \
    src/base/contextualizationmodel.h \
    src/tools/utils.h \
    src/tools/ocr.h \
    src/gui/guicontextualizationcontroller.h \
    src/base/contextualizationcontrollerbase.h \
    src/console/consolecontextualizationcontroller.h

DISTFILES +=
