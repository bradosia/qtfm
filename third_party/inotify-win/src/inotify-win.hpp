/*
 * Parts Used From linux/include/uapi/linux/inotify.h
 * Inode based directory notification for Linux
 * Copyright (C) 2005 John McCutchan
 */

#ifndef INOTIFY_WIN_H
#define INOTIFY_WIN_H

#define DEBUG_INOTIFY_WIN 1

// c++11
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

// windows
#include <windows.h>

/*
 * struct inotify_event - structure read from the inotify device for each event
 *
 * When you are watching a directory, you will receive the filename for events
 * such as IN_CREATE, IN_DELETE, IN_OPEN, IN_CLOSE, ..., relative to the wd.
 */
struct inotify_event {
  int32_t wd;      // watch descriptor returned by inotify_add_watch()
  uint32_t mask;   // A bitmask that includes the event type
  uint32_t cookie; // An unique number that identifies related events
  uint32_t len; // The length of the name field, including any required padding.
  char name[0]; // The name of the object that the event occurred to.
};

/* the following are legal, implemented events that user-space can watch for */
#define IN_ACCESS 0x00000001        /* File was accessed */
#define IN_MODIFY 0x00000002        /* File was modified */
#define IN_ATTRIB 0x00000004        /* Metadata changed */
#define IN_CLOSE_WRITE 0x00000008   /* Writtable file was closed */
#define IN_CLOSE_NOWRITE 0x00000010 /* Unwrittable file closed */
#define IN_OPEN 0x00000020          /* File was opened */
#define IN_MOVED_FROM 0x00000040    /* File was moved from X */
#define IN_MOVED_TO 0x00000080      /* File was moved to Y */
#define IN_CREATE 0x00000100        /* Subfile was created */
#define IN_DELETE 0x00000200        /* Subfile was deleted */
#define IN_DELETE_SELF 0x00000400   /* Self was deleted */
#define IN_MOVE_SELF 0x00000800     /* Self was moved */

/* the following are legal events.  they are sent as needed to any watch */
#define IN_UNMOUNT 0x00002000    /* Backing fs was unmounted */
#define IN_Q_OVERFLOW 0x00004000 /* Event queued overflowed */
#define IN_IGNORED 0x00008000    /* File was ignored */

/* helper events */
#define IN_CLOSE (IN_CLOSE_WRITE | IN_CLOSE_NOWRITE) /* close */
#define IN_MOVE (IN_MOVED_FROM | IN_MOVED_TO)        /* moves */

/* special flags */
#define IN_ONLYDIR 0x01000000     /* only watch the path if it is a directory */
#define IN_DONT_FOLLOW 0x02000000 /* don't follow a sym link */
#define IN_EXCL_UNLINK 0x04000000 /* exclude events on unlinked objects */
#define IN_MASK_CREATE 0x10000000 /* only create watches */
#define IN_MASK_ADD                                                            \
  0x20000000                  /* add to the mask of an already existing watch */
#define IN_ISDIR 0x40000000   /* event occurred against dir */
#define IN_ONESHOT 0x80000000 /* only send event once */

/*
 * All of the events - we build the list by hand so that we can add flags in
 * the future and not break backward compatibility.  Apps will get only the
 * events that they originally wanted.  Be sure to add new events here!
 */
#define IN_ALL_EVENTS                                                          \
  (IN_ACCESS | IN_MODIFY | IN_ATTRIB | IN_CLOSE_WRITE | IN_CLOSE_NOWRITE |     \
   IN_OPEN | IN_MOVED_FROM | IN_MOVED_TO | IN_DELETE | IN_CREATE |             \
   IN_DELETE_SELF | IN_MOVE_SELF)

class InotifyHandle {
public:
  int fd; // file descriptor
  std::vector<HANDLE> hDirectoryList; // directory change handle list
};

int inotify_init();
int inotify_add_watch(int fd, const char *pathname, int mask);
int inotify_rm_watch(int fd, int32_t wd);
int ioctl(int fd, int request, ...);

#endif