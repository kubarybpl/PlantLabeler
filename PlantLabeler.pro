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
    dataset.h \
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

############

INCLUDEPATH += D:/libtorch/include
INCLUDEPATH += D:/libtorch/include/torch/csrc/api/include
#INCLUDEPATH += D:/libtorch/include/torch/csrc/api/include
LIBS += D:/libtorch/lib/torch.lib
LIBS += D:/libtorch/lib/torch_cpu.lib
LIBS += D:/libtorch/lib/c10.lib

########################

# Ścieżki do plików nagłówkowych TensorFlow

#INCLUDEPATH += C:/Users/Kuba/tensorflow/bazel-bin/tensorflow/include
#INCLUDEPATH += C:/Users/Kuba/tensorflow/bazel-bin/tensorflow/include/src
#INCLUDEPATH += C:/Users/Kuba/tensorflow/bazel-bin
#INCLUDEPATH += C:/Users/Kuba/tensorflow/bazel-bin/external
#INCLUDEPATH += C:/Users/Kuba/tensorflow/bazel-tensorflow/external/com_google_protobuf
#INCLUDEPATH += C:/Users/Kuba/tensorflow/bazel-tensorflow/external/com_google_protobuf/src

#DEPENDPATH += C:/Users/Kuba/tensorflow/bazel-bin/tensorflow
#DEPENDPATH += C:/Users/Kuba/tensorflow/bazel-bin/tensorflow/cc
#DEPENDPATH += C:/Users/Kuba/tensorflow/bazel-bin/tensorflow/cc/ops


## Ścieżki do bibliotek TensorFlow
#LIBS += C:/Users/Kuba/tensorflow/bazel-bin/tensorflow/tensorflow.lib
#LIBS += C:/Users/Kuba/tensorflow/bazel-bin/tensorflow/tensorflow_cc.lib




#########################

LIBS += D:\opencv_MSVC\opencv\build\install\x64\vc16\lib\opencv_core490.lib
LIBS += D:\opencv_MSVC\opencv\build\install\x64\vc16\lib\opencv_video490.lib
LIBS += D:\opencv_MSVC\opencv\build\install\x64\vc16\lib\opencv_videoio490.lib
LIBS += D:\opencv_MSVC\opencv\build\install\x64\vc16\lib\opencv_highgui490.lib
LIBS += D:\opencv_MSVC\opencv\build\install\x64\vc16\lib\opencv_imgproc490.lib
LIBS += D:\opencv_MSVC\opencv\build\install\x64\vc16\lib\opencv_imgcodecs490.lib

INCLUDEPATH += D:\opencv_MSVC\opencv\build\include
DEPENDPATH += D:\opencv_MSVC\opencv\build\include

RESOURCES += \
    icons.qrc
