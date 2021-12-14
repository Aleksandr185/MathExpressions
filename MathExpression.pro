QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    expression_widget.cpp \
    main.cpp \
    src/abstract_big_parent_expression.cpp \
    src/abstract_expression.cpp \
    src/abstract_parent_expression.cpp \
    src/asterix_expression.cpp \
    src/at_value_expression.cpp \
    src/bracketed_expression.cpp \
    src/cap_expression.cpp \
    src/chain_expression.cpp \
    src/character_expression.cpp \
    src/comma_expression.cpp \
    src/common_function_expression.cpp \
    src/custom_text_expression.cpp \
    src/empty_expression.cpp \
    src/extended_number_expresssion.cpp \
    src/fraction_expression.cpp \
    src/function_expression.cpp \
    src/function_name_expression.cpp \
    src/lambda_expression.cpp \
    src/nabla_expression.cpp \
    src/number_expresssion.cpp \
    src/planck_expression.cpp \
    src/root_expression.cpp \
    src/round_bracketed_expression.cpp \
    src/simple_expression.cpp \
    src/space_expression.cpp \
    src/strokes_expression.cpp \
    src/variable_expression.cpp \
    widget.cpp

HEADERS += \
    expression_widget.h \
    src/abstract_big_parent_expression.h \
    src/abstract_expression.h \
    src/abstract_parent_expression.h \
    src/asterix_expression.h \
    src/at_value_expression.h \
    src/bracketed_expression.h \
    src/cap_expression.h \
    src/chain_expression.h \
    src/character_expression.h \
    src/comma_expression.h \
    src/common_function_expression.h \
    src/custom_text_expression.h \
    src/empty_expression.h \
    src/extended_number_expresssion.h \
    src/fraction_expression.h \
    src/function_expression.h \
    src/function_name_expression.h \
    src/global.h \
    src/lambda_expression.h \
    src/nabla_expression.h \
    src/number_expresssion.h \
    src/planck_expression.h \
    src/root_expression.h \
    src/round_bracketed_expression.h \
    src/simple_expression.h \
    src/space_expression.h \
    src/strokes_expression.h \
    src/variable_expression.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
