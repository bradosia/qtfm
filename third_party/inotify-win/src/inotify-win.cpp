#include "inotify-win.hpp"
/* POSIX operating system API
 * Cygwin and MinGW provide their own version
 */
#include <errno.h>
#include <fcntl.h> // open
#include <thread>

// Global Variables
int InotifyHandle_index = 1;
int inotify_directory_handle_index = 1;
std::unordered_map<int, InotifyHandle *> InotifyHandle_map;
std::unordered_map<int, HANDLE> inotify_directory_handle_map;

// 0.3 seconds
constexpr std::chrono::duration<int, std::micro> ms300(3000000);
// 0.1 seconds for time waster.
constexpr std::chrono::duration<int, std::micro> ms100(1000000);

void taskloop(int fd) {
  // just 10 iterations. Replace with proper termination condition
  for (int count = 0; count < 10; count++) {
    auto start = std::chrono::high_resolution_clock::now();
    // calculate result
    std::this_thread::sleep_for(ms100); // time waster for test

    std::string test = "hello";
    //write(fd, test.c_str(), test.length());
    send(fd, test.c_str(), test.length(), 0);
#ifdef DEBUG_INOTIFY_WIN
    std::cout << "taskloop(" << fd << ")\ntest:" << test << "\n";
#endif

    // output result.
    std::cout << start.time_since_epoch().count() << ":" << count << std::endl;
    auto delta = std::chrono::high_resolution_clock::now() - start;
    auto sleeptime = ms300 - delta;
    std::this_thread::sleep_for(sleeptime);
    char buf[50];
    read(fd, buf, 50);
#ifdef DEBUG_INOTIFY_WIN
    std::cout << "taskloop(" << fd << ")\nbuf:" << buf << "\n";
#endif
  }
}

int inotify_init() {
  // return code of 0 reserved
  std::string filename = "~$" + std::to_string(InotifyHandle_index);
  // int fd = open(filename.c_str(), O_RDWR | O_CREAT);
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    std::cout << "socket() failed: " << strerror(errno) << "\n";
    return -1;
  }
  InotifyHandle *inotify_ptr = new InotifyHandle();
  inotify_ptr->fd = fd;
  InotifyHandle_map.insert({fd, inotify_ptr});
#ifdef DEBUG_INOTIFY_WIN
  std::cout << "inotify_init()\nfilename:" << filename << "\nfd:" << fd << "\n";
#endif
  InotifyHandle_index++;

  // start thread and call taskloop
  std::thread testthread(taskloop, fd);
  // wait for thread to end.
  testthread.detach();
  /*
  for (HANDLE hDirectory : inotify_ptr->hDirectoryList) {
    LPVOID *lpBuffer;
    DWORD nBufferLength;
    BOOL bWatchSubtree = true;
    DWORD dwNotifyFilter;
    LPDWORD lpBytesReturned;
    LPOVERLAPPED lpOverlapped;
    LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine;

    BOOL flag = ReadDirectoryChangesW(
        hDirectory, lpBuffer, nBufferLength, bWatchSubtree, dwNotifyFilter,
        lpBytesReturned, lpOverlapped, lpCompletionRoutine);
    if (flag) {
      return -1;
    }
    write(fd, lpBuffer, nBufferLength);
  }*/

  return fd;
}

int inotify_add_watch(int fd, const char *pathname, int mask) {
#ifdef DEBUG_INOTIFY_WIN
  std::cout << "inotify_add_watch(" << fd << "," << pathname << "," << mask
            << ")\n";
#endif
  auto findFlag = InotifyHandle_map.find(fd);
  if (findFlag == InotifyHandle_map.end())
    return -1;
  InotifyHandle *inotify_ptr = InotifyHandle_map.at(fd);
  // char* to WCHAR* (LPWSTR)
  LPWSTR lpPathName;
  int nChars = MultiByteToWideChar(CP_ACP, 0, pathname, -1, NULL, 0);
  lpPathName = new WCHAR[nChars];
  MultiByteToWideChar(CP_ACP, 0, pathname, -1, (LPWSTR)lpPathName, nChars);
#ifdef DEBUG_INOTIFY_WIN
  std::cout << "lpPathName:" << lpPathName << "\n";
#endif
  // other options
  bool bWatchSubtree = true;
  DWORD dwNotifyFilter =
      FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
      FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE |
      FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SECURITY;
  HANDLE directory_change_handle =
      FindFirstChangeNotificationW(lpPathName, bWatchSubtree, dwNotifyFilter);
  // if the function fails, the return value is INVALID_HANDLE_VALUE
  if (directory_change_handle == INVALID_HANDLE_VALUE) {
    return -1;
  }
  // inotify_ptr->directory_change_handle = directory_change_handle;
  inotify_directory_handle_map.insert(
      {inotify_directory_handle_index, directory_change_handle});
  inotify_ptr->hDirectoryList.push_back(directory_change_handle);
  // deletes
  delete[] lpPathName;
  // return wd = watch descriptor, or -1 if an error occurred
  return inotify_directory_handle_index++;
}

int inotify_rm_watch(int fd, int32_t wd) {
#ifdef DEBUG_INOTIFY_WIN
  std::cout << "inotify_rm_watch(" << fd << "," << wd << ")\n";
#endif
  auto findFlag = inotify_directory_handle_map.find(fd);
  if (findFlag == inotify_directory_handle_map.end())
    return -1;
  HANDLE directory_change_handle = inotify_directory_handle_map.at(wd);
  bool flag = FindCloseChangeNotification(directory_change_handle);
  if (flag) {
    return -1;
  }
  return 0;
}

int ioctl(int fd, int request, ...) {
#ifdef DEBUG_INOTIFY_WIN
  std::cout << "ioctl(" << fd << "," << request << ")\n";
#endif
  auto findFlag = InotifyHandle_map.find(fd);
  if (findFlag == InotifyHandle_map.end())
    return -1;
  InotifyHandle *inotify_ptr = InotifyHandle_map.at(fd);
  switch (request) {
  case FIONREAD:
    for (HANDLE hDirectory : inotify_ptr->hDirectoryList) {
      LPVOID *lpBuffer;
      DWORD nBufferLength;
      BOOL bWatchSubtree = true;
      DWORD dwNotifyFilter;
      LPDWORD lpBytesReturned;
      LPOVERLAPPED lpOverlapped;
      LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine;

      BOOL flag = ReadDirectoryChangesW(
          hDirectory, lpBuffer, nBufferLength, bWatchSubtree, dwNotifyFilter,
          lpBytesReturned, lpOverlapped, lpCompletionRoutine);
      if (flag) {
        return -1;
      }
      write(fd, lpBuffer, nBufferLength);
    }
    break;
  }
  return 0;
}
