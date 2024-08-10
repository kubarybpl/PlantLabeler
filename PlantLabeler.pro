QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customproxymodel.cpp \
    interactivescene.cpp \
    leftpanel.cpp \
    main.cpp \
    mainwindow.cpp \
    rightpanel.cpp

HEADERS += \
    customproxymodel.h \
    interactivescene.h \
    leftpanel.h \
    mainwindow.h \
    rightpanel.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#win32:CONFIG(release, debug|release): LIBS += -LD:/opencv/build/x64/vc16/lib/ -lopencv_world490
#else:win32:CONFIG(debug, debug|release): LIBS += -LD:/opencv/build/x64/vc16/lib/ -lopencv_world490d

LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_core490.dll
LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_video490.dll
LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_videoio490.dll
LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_highgui490.dll
LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_imgproc490.dll
LIBS += D:\opencv\build\install\x64\mingw\bin\libopencv_imgcodecs490.dll

INCLUDEPATH += D:\opencv\build\include
DEPENDPATH += D:\opencv\build\include

#LIBS += D:\opencv_minGW\bin\libopencv_core490.dll

#LIBS += -LD:\\opencv_minGW\\install\\x64\\mingw\\bin \
#    libopencv_core490 \
#    libopencv_highgui490d \
#    libopencv_imgproc490d \


#INCLUDEPATH += D:\opencv_minGW\include
#LIBS += -LD:\opencv_minGW\lib \
#        -lopencv_core490 \
#        -lopencv_highgui490 \
#        -lopencv_imgproc490 \
#        -lopencv_imgcodecs490 \
#        -lopencv_videoio490

RESOURCES += \
    icons.qrc
