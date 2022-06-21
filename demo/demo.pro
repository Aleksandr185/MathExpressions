QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = MathExpressionsDemo
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# ---------------------------------------- BuildConfig-- -------------------------------------------
include($$PWD/build_config.pri)

message(DEST_DIR: $${DEST_DIR})
message(MOC_DIR: $${MOC_DIR})
message(OBJECTS_DIR: $${OBJECTS_DIR})
message(Qt version: $${QT_VERSION})
message(Arch Type : $${ARCH_TYPE})

# -------------------------------------- MathExpressions -------------------------------------------
include($$PWD/../src/math_expressions.pri)

SOURCES += \
    expression_widget.cpp \
    main.cpp \
    widget_demo.cpp \
    widget_help.cpp

HEADERS += \
    expression_widget.h \
    widget_demo.h \
    widget_help.h

## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
