QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    custom_view_frame.cpp \
    Frame_manager.cpp \
    frame_item.cpp

HEADERS += \
    mainwindow.h \
    custom_view_frame.h \
    Frame_manager.h \
    frame_data.h \
    frame_item.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_core -lopencv_cvv -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

INCLUDEPATH += $$PWD/../../../../usr/local/include/opencv4
DEPENDPATH += $$PWD/../../../../usr/local/include/opencv4
