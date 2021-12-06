QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/abstract_expression.cpp \
    src/custom_text_expression.cpp \
    src/simple_expression.cpp \
    src/variable_expression.cpp \
    widget.cpp

HEADERS += \
    src/abstract_expression.h \
    src/custom_text_expression.h \
    src/global.h \
    src/simple_expression.h \
    src/variable_expression.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
