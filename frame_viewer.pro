QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    custom_dialog_frame_settings.cpp \
    custom_frame_dialog.cpp \
    custom_graphic_scene.cpp \
    main.cpp \
    mainwindow.cpp \
    custom_view_frame.cpp \
    Frame_manager.cpp \
    frame_item.cpp

HEADERS += \
    custom_dialog_frame_settings.h \
    custom_frame_dialog.h \
    custom_graphic_scene.h \
    mainwindow.h \
    custom_view_frame.h \
    Frame_manager.h \
    frame_data.h \
    frame_item.h

FORMS += \
    custom_dialog_frame_settings.ui \
    custom_frame_dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib64/ -lopencv_core -lopencv_cvv -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc
#ubuntu local/ib
#redos local/lib64
INCLUDEPATH += $$PWD/../../../../usr/local/include/opencv4
DEPENDPATH += $$PWD/../../../../usr/local/include/opencv4
