QT += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    classes/atom.cpp \
    classes/parse_file.cpp \
    main.cpp \
    mainwindow.cpp \
    widgets/molecule_visualization_widget.cpp

HEADERS += \
    classes/atom.h \
    classes/parse_file.h \
    mainwindow.h \
    widgets/molecule_visualization_widget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    shaders/testShader.vert \
    shaders/testShader2.fsh

RESOURCES += \
    shaders.qrc
