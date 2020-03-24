#include "vfs-win.hpp"

int statfs(const char *path, struct statfs *buf) {
  // char* to WCHAR* (LPWSTR)
  LPWSTR lpDirectoryName;
  int nChars = MultiByteToWideChar(CP_ACP, 0, path, -1, NULL, 0);
  lpDirectoryName = new WCHAR[nChars];
  MultiByteToWideChar(CP_ACP, 0, path, -1, (LPWSTR)lpDirectoryName, nChars);
  // other options
  ULARGE_INTEGER lpFreeBytesAvailableToCaller, lpTotalNumberOfBytes,
      lpTotalNumberOfFreeBytes;
  BOOL flag = GetDiskFreeSpaceExW(
      lpDirectoryName, (PULARGE_INTEGER)&lpFreeBytesAvailableToCaller,
      (PULARGE_INTEGER)&lpTotalNumberOfBytes,
      (PULARGE_INTEGER)&lpTotalNumberOfFreeBytes);
#ifdef DEBUG_VFS_WIN
  std::cout << "statfs(" << path
            << ")\nf_blocks:" << lpTotalNumberOfBytes.QuadPart
            << "\nf_bfree:" << lpTotalNumberOfFreeBytes.QuadPart << "\nf_bavail"
            << lpFreeBytesAvailableToCaller.QuadPart << "\n";
#endif
  buf->f_bsize = 512;
  buf->f_blocks = lpTotalNumberOfBytes.QuadPart;
  buf->f_bfree = lpTotalNumberOfFreeBytes.QuadPart;
  buf->f_bavail = lpFreeBytesAvailableToCaller.QuadPart;
  if (flag == 0)
    return -1;
  return 0;
}
/*
int fstatfs(int, struct statfs *){

}
*/
