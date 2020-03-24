QT += widgets concurrent

TARGET = ioctl-win
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    src/ioctl-win.cpp

HEADERS += \
    src/ioctl-win.hpp

DESTDIR = $${top_builddir}/lib$${LIBSUFFIX}
