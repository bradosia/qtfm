/*
 * Parts Used From android/platform/bionic/froyo/./libc/include/sys/vfs.h
 * Copyright (C) 2008 The Android Open Source Project
 */

#ifndef VFS_WIN_H
#define VFS_WIN_H

#define DEBUG_VFS_WIN 1

// c
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

// c++11
#include <iostream>

// windows
#include <windows.h>

/* note: this corresponds to the kernel's statfs64 type */
struct statfs {
    //uint32_t        f_type;
    uint32_t        f_bsize; /* Optimal transfer block size */
    uint64_t        f_blocks; /* Total data blocks in filesystem */
    uint64_t        f_bfree; /* Free blocks in filesystem */
    uint64_t        f_bavail; /* Free blocks available to unprivileged user */
    //uint64_t        f_files;
    //uint64_t        f_ffree;
    //uint32_t        f_namelen;
    //uint32_t        f_frsize;
    //uint32_t        f_spare[5];
};
#define  ADFS_SUPER_MAGIC      0xadf5
#define  AFFS_SUPER_MAGIC      0xADFF
#define  BEFS_SUPER_MAGIC      0x42465331
#define  BFS_MAGIC             0x1BADFACE
#define  CIFS_MAGIC_NUMBER     0xFF534D42
#define  CODA_SUPER_MAGIC      0x73757245
#define  COH_SUPER_MAGIC       0x012FF7B7
#define  CRAMFS_MAGIC          0x28cd3d45
#define  DEVFS_SUPER_MAGIC     0x1373
#define  EFS_SUPER_MAGIC       0x00414A53
#define  EXT_SUPER_MAGIC       0x137D
#define  EXT2_OLD_SUPER_MAGIC  0xEF51
#define  EXT2_SUPER_MAGIC      0xEF53
#define  EXT3_SUPER_MAGIC      0xEF53
#define  HFS_SUPER_MAGIC       0x4244
#define  HPFS_SUPER_MAGIC      0xF995E849
#define  HUGETLBFS_MAGIC       0x958458f6
#define  ISOFS_SUPER_MAGIC     0x9660
#define  JFFS2_SUPER_MAGIC     0x72b6
#define  JFS_SUPER_MAGIC       0x3153464a
#define  MINIX_SUPER_MAGIC     0x137F /* orig. minix */
#define  MINIX_SUPER_MAGIC2    0x138F /* 30 char minix */
#define  MINIX2_SUPER_MAGIC    0x2468 /* minix V2 */
#define  MINIX2_SUPER_MAGIC2   0x2478 /* minix V2, 30 char names */
#define  MSDOS_SUPER_MAGIC     0x4d44
#define  NCP_SUPER_MAGIC       0x564c
#define  NFS_SUPER_MAGIC       0x6969
#define  NTFS_SB_MAGIC         0x5346544e
#define  OPENPROM_SUPER_MAGIC  0x9fa1
#define  PROC_SUPER_MAGIC      0x9fa0
#define  QNX4_SUPER_MAGIC      0x002f
#define  REISERFS_SUPER_MAGIC  0x52654973
#define  ROMFS_MAGIC           0x7275
#define  SMB_SUPER_MAGIC       0x517B
#define  SYSV2_SUPER_MAGIC     0x012FF7B6
#define  SYSV4_SUPER_MAGIC     0x012FF7B5
#define  TMPFS_MAGIC           0x01021994
#define  UDF_SUPER_MAGIC       0x15013346
#define  UFS_MAGIC             0x00011954
#define  USBDEVICE_SUPER_MAGIC 0x9fa2
#define  VXFS_SUPER_MAGIC      0xa501FCF5
#define  XENIX_SUPER_MAGIC     0x012FF7B4
#define  XFS_SUPER_MAGIC       0x58465342
#define  _XIAFS_SUPER_MAGIC    0x012FD16D
int statfs(const char *, struct statfs *);
//int fstatfs(int, struct statfs *);

#endif
