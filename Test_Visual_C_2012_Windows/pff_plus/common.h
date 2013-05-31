#ifndef COMMON_H
#define COMMON_H

#define flash

#define PSTR(x) (x)
#define CONST

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long	DWORD;

#define memcpyf memcpy
#define strcpyf(A,B) strcpy((char*)(A), (char*)(B))

#pragma warning(disable:4996)

#define X86_DEBUG

#endif