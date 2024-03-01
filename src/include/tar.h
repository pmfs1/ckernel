//
// Extended tar definitions
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef TAR_H
#define TAR_H

//
// General definitions
//

#define TMAGIC     "ustar"  // ustar plus null byte
#define TMAGLEN    6        // Length of the above
#define TVERSION   "00"     // 00 without a null byte
#define TVERSLEN   2        // Length of the above

//
// Type flags
//

#define REGTYPE    '0'      // Regular file
#define AREGTYPE   '\0'     // Regular file
#define LNKTYPE    '1'      // Link
#define SYMTYPE    '2'      // Symbolic link
#define CHRTYPE    '3'      // Character special
#define BLKTYPE    '4'      // Block special
#define DIRTYPE    '5'      // Directory
#define FIFOTYPE   '6'      // FIFO special
#define CONTTYPE   '7'      // Reserved

//
// Mode field bits
//

#define TSUID      04000    // Set UID on execution
#define TSGID      02000    // Set GID on execution
#define TSVTX      01000    // On directories, restricted deletion flag
#define TUREAD     00400    // Read by owner
#define TUWRITE    00200    // Write by owner special
#define TUEXEC     00100    // Execute/search by owner
#define TGREAD     00040    // Read by group
#define TGWRITE    00020    // Write by group
#define TGEXEC     00010    // Execute/search by group
#define TOREAD     00004    // Read by other
#define TOWRITE    00002    // Write by other
#define TOEXEC     00001    // Execute/search by other

#define TAR_BLKSIZ     512  // Standard tar block size
#define TAR_NAMELEN    100  // Name length
#define TAR_PREFIXLEN  155  // Prefix length
#define TAR_MAXPATHLEN (TAR_NAMELEN + TAR_PREFIXLEN)

//
// Tar header
//

struct tarhdr {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];
};

#endif
