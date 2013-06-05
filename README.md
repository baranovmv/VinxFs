It is an open source software to implement FAT file system to small embedded systems. This is a free software and 
is opened for education, research and commercial developments under license policy of following trems.

(C) 2013 vinxru

(C) 2010, ChaN, all right reserved.

*** Features ***

- Very small RAM consumption (~50 bytes work area + certain stack)
- 512 byte temporary buffer. Used only while the function run.
- Very small code size (2K-7K bytes).
- Supports FAT16, FAT32.
- Single volume and two opened files.
- File write function with out restrictions.
- Can create, delete, rename, resize, move, file and directory

*** Application Interface ***

VinxFS module provides following functions:

- fs_init - Mount a volume
- fs_check - If user changed a drive, when mount it
- fs_opendir - Open a directory
- fs_readdir - Read a directory item
- fs_delete - Delete a file or directory
- fs_open - Open a File
- fs_openany - Open a file or directory
- fs_create - Create a file;
- fs_createdir - Create a directory
- fs_move0, fs_move - Move or rename a file or directory
- fs_lseek - Move read/write pointer
- fs_tell - Get read/write pointer
- fs_getfilesize - Get file size
- fs_read0, fs_read - Read file
- fs_write - Write File
- fs_write_eof - Truncate file
- fs_write_start, fs_write_end - Write temporary buffer to a file
- fs_swap - Swap opened files (work with two files)
- fs_gettotal - Get total drive space
- fs_getfree - Get free drive space

*** Drive I/O Interface ***

Since the VinxFS module is completely separated from disk I/O layer, it requires following functions to 
lower layer to read the physical disk.

- sd_init - Initialize disk drive
- sd_check - Check if user changed a drive
- sd_read - Read partial sector
- sd_write512 - Write sector

The module contains driver SD Card connected at ATMega8
