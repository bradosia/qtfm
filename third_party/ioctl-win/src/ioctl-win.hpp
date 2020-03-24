/*
 * Parts Used From linux/include/uapi/linux/inotify.h
 * Inode based directory notification for Linux
 * Copyright (C) 2005 John McCutchan
 */

#ifndef IOCTL_WIN_H
#define IOCTL_WIN_H

#define DEBUG_IOCTL_WIN 1

// windows
#include <windows.h>

int ioctl(int fd, int request, ...);

#endif
