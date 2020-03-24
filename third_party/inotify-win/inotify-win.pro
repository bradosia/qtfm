QT += widgets concurrent

TARGET = inotify-win
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    src/inotify-win.cpp

HEADERS += \
    src/inotify-win.hpp

DESTDIR = $${top_builddir}/lib$${LIBSUFFIX}
