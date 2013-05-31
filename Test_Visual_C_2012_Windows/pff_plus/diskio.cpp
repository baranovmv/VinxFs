/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2009      */
/*-----------------------------------------------------------------------*/

#include <stdafx.h>
#include "sd.h"
#include "fs.h"
#include <Windows.h>

HANDLE handle = INVALID_HANDLE_VALUE;

BYTE sd_init() {
  handle = CreateFile(L"fat16.vhd", GENERIC_WRITE|GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
  if(handle==INVALID_HANDLE_VALUE) { lastError = ERR_DISK_ERR; return 1; }
	return 0;
}

BYTE sd_check() {
  return handle==INVALID_HANDLE_VALUE;
}

BYTE sd_read(BYTE* dest,	DWORD sector,	WORD sofs, WORD count) {
  if(SetFilePointer(handle, sector*512+sofs, 0, FILE_BEGIN)==INVALID_SET_FILE_POINTER) { lastError=ERR_DISK_ERR; return 1; }
  DWORD tmp;
  if(!ReadFile(handle, dest, count, &tmp, 0) || tmp!=count) { lastError=ERR_DISK_ERR; return 1; }
	return 0;
}

BYTE sd_write512(BYTE* dest,	DWORD sect) {
  if(SetFilePointer(handle, sect*512, 0, FILE_BEGIN)==INVALID_SET_FILE_POINTER) { lastError=ERR_DISK_ERR; return 1; }
  DWORD tmp;
  if(!WriteFile(handle, dest, 512, &tmp, 0) || tmp!=512) { lastError=ERR_DISK_ERR; return 1; }
  return 0;
}
