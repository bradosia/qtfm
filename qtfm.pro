TEMPLATE = subdirs
CONFIG -= ordered
SUBDIRS += libfm
fm.depends += libfm

win32-g++ {
    SUBDIRS += third_party/inotify-win \
        third_party/ioctl-win \
        third_party/vfs-win
}

SUBDIRS += fm

CONFIG(v7) {
    SUBDIRS += fm7
    fm7.depends += libfm
}

unix:!macx {
    !CONFIG(no_dbus) {
        !CONFIG(no_tray) {
            SUBDIRS += tray
            tray.depends += libfm
        }
    }
}
