QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = PlantLabeler

SOURCES += \
    customproxymodel.cpp \
    interactivescene.cpp \
    leftpanel.cpp \
    main.cpp \
    mainwindow.cpp \
    neuralnetworkdialog.cpp \
    rightpanel.cpp

HEADERS += \
    customproxymodel.h \
    interactivescene.h \
    leftpanel.h \
    mainwindow.h \
    neuralnetworkdialog.h \
    rightpanel.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_core490.dll
LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_video490.dll
LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_videoio490.dll
LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_highgui490.dll
LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_imgproc490.dll
LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_imgcodecs490.dll

INCLUDEPATH += D:\opencv\build\include
DEPENDPATH += D:\opencv\build\include


RESOURCES += \
    icons.qrc
