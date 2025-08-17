TEMPLATE = lib
CONFIG += plugin
QT += widgets

TARGET = clangformatplugin

INCLUDEPATH += ../../

SOURCES += \
    clangformatplugin.cpp \
    clangformatsettingswidget.cpp

HEADERS += \
    clangformatplugin.h \
    clangformatsettingswidget.h

# Ensure plugin headers are visible to host includes
DISTFILES += \
    README.md
