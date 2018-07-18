QT += quick
QT += widgets
CONFIG += c++11

#Include root path of personal classes.
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
    src/contextualization/model/contextualizationmodel.cpp \
    src/contextualization/model/firmwarestring.cpp \
    src/optical_character_recognition/ocr.cpp \
    src/tools/log.cpp \
    src/tools/utils.cpp \
    src/contextualization/controller/consolecontroller.cpp \
    src/contextualization/controller/contextualizationcontroller.cpp \
    src/contextualization/controller/guicontroller.cpp \
    src/contextualization/controller/linuxguicontroller.cpp \
    src/contextualization/controller/windowsguicontroller.cpp \
    src/contextualization/controller/linuxconsolecontroller.cpp \
    src/contextualization/controller/windowsconsolecontroller.cpp \
    src/optical_character_recognition/tesseractocr.cpp \
    src/tools/contextualizationfactoryabstract.cpp \
    src/tools/hpcontextualizationfactory.cpp \
    src/contextualization/model/string.cpp \
    src/storage/mysqlconnector.cpp \
    src/storage/databaseconnectorabstract.cpp \
    src/storage/fpfileconnector.cpp

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
    src/contextualization/model/contextualizationmodel.h \
    src/contextualization/model/firmwarestring.h \
    src/optical_character_recognition/ocr.h \
    src/tools/log.h \
    src/tools/utils.h \
    src/contextualization/controller/consolecontroller.h \
    src/contextualization/controller/contextualizationcontroller.h \
    src/contextualization/controller/guicontroller.h \
    src/contextualization/controller/linuxguicontroller.h \
    src/contextualization/controller/windowsguicontroller.h \
    src/contextualization/controller/linuxconsolecontroller.h \
    src/contextualization/controller/windowsconsolecontroller.h \
    src/optical_character_recognition/tesseractocr.h \
    src/tools/contextualizationfactoryabstract.h \
    src/tools/hpcontextualizationfactory.h \
    src/contextualization/model/string.h \
    src/storage/mysqlconnector.h \
    src/storage/databaseconnectorabstract.h \
    src/storage/fpfileconnector.h

DISTFILES +=
