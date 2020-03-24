#include "ioctl-win.hpp"
/* POSIX operating system API
 * Cygwin and MinGW provide their own version
 */
#include <fcntl.h> // open
#include <iostream>

int ioctl(int fd, int request, ...) {
#ifdef DEBUG_IOCTL_WIN
  std::cout << "ioctl(" << fd << "," << request << ")\n";
#endif
  switch (request) {
  case FIONREAD:
    int off = lseek(fd, 0L, SEEK_END);
    lseek(fd, 0L, SEEK_SET);
    va_list args;
    va_start(args, request);
    int *arg3 = va_arg(args, int *);
    (*arg3) = off;
    va_end(args);
    break;
  }
  return 0;
}
