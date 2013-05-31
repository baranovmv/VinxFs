/*
It is an open source software to implement FAT file system to
small embedded systems. This is a free software and is opened for education,
research and commercial developments under license policy of following trems.

(C) 2013 vinxru
(C) 2010, ChaN, all right reserved.

It is a free software and there is NO WARRANTY.
No restriction on use. You can use, modify and redistribute it for
personal, non-profit or commercial use UNDER YOUR RESPONSIBILITY.
Redistributions of source code must retain the above copyright notice.

Version 0.99 5-05-2013
*/

#ifndef _FS_H_
#define _FS_H_

#include "common.h"

/* Disable checking and kill the file system */
/* С помощью этого макроса можно отключить проверки, что бы сэкономить ПЗУ и убить файловую систему.*/
/* #define FS_DISABLE_CHECK */

/* Function fs_swap (Two opened files) */
/* Функция fs_swap (два открытых файла) */
/* #define FS_DISABLE_SWAP */

/* Function fs_getfree (Calculation of free disk space) */
/* Функция fs_getfree (определение свободного места на диске) */
/* #define FS_DISABLE_GETFREESPACE */

/* Work buffer for filesystem */
/* Буфер для работы файловой системы */
extern BYTE buf[512];

/* File syustem errors */
/* Ошибки файловой системы */

#define ERR_OK              0  // Нет ошибки
#define ERR_NO_FILESYSTEM   1  // Файловая система не обнаружена
#define ERR_DISK_ERR        2  // Ошибка чтения/записи
#define	ERR_NOT_OPENED      3  // Файл/папка не открыта
#define	ERR_NO_PATH         4  // Файл/папка не найдена
#define ERR_DIR_FULL        5  // Папка содержит максимальное кол-во файлов
#define ERR_NO_FREE_SPACE   6  // Нет свободного места
#define ERR_DIR_NOT_EMPTY   7  // Нельзя удалить папку, она не пуста
#define ERR_FILE_EXISTS     8  // Файл/папка с таким именем уже существует
#define ERR_NO_DATA         9  // fs_file_wtotal=0 при вызове функции fs_write_begin
    
#define ERR_MAX_FILES       10 // Не используется файловой системой, резерв
#define ERR_RECV_STRING     11 // Не используется файловой системой, резерв
#define ERR_INVALID_COMMAND 12 // Не используется файловой системой, резерв
         
/* Filesystem variables. Read-only */
/* Переменные файловой системы. Только для чтения */

typedef struct { 
#ifndef FS_DISABLE_CHECK
  BYTE  opened;              /* Что открыто: OPENED_NONE, OPENED_FILE, OPENED_DIR */
#endif
  BYTE  entry_able;          /* Результат выполнения функции fs_dirread */
  WORD  entry_index;         /* Номер записи в каталоге */
  DWORD entry_cluster;       /* Кластер записи в каталоге */
  DWORD entry_sector;        /* Сектор записи в каталоге */
  DWORD entry_start_cluster; /* Первый сектор файла или каталога (0 - корневой каталог FAT16) */
  DWORD ptr;                 /* Указатель чтения/записи файла*/
  DWORD size;                /* Размер файла / File size */
  DWORD cluster;             /* Текущий кластер файла / Current cluster */
  DWORD sector;              /* Текущий сектор файла / Current sector */
  WORD  wtotal;              /* Аргументы функции fs_write_start, fs_write_end */
  BYTE  changed;             /* Размер файла изменился, надо сохранить */
} File;

extern File fs_file;
extern DWORD fs_tmp;

/* Filesystem variables. You can change it. */
/* Переменные файловой системы. Можно изменять */

extern BYTE lastError;       /* Последняя ошибка файловой системы или диска / Last error */

/* Maximal length of file name */
/* Максимальная длина имени */

#define FS_MAXFILE  469

/* Result of fs_readdir */
/* Описатель прочитанный функцией fs_readdir */

#define FS_DIRENTRY  (buf + 480)

/* Functions. Returns not 0 if error occured. Destroy buf variable*/
/* Функцяии. Возращают не 0, если ошибка. Все функции портят buf */

BYTE fs_init();                                  /* Инициалиазция файловой системы / Init filesystem */
BYTE fs_check();                                 /* Проверка наличия диска, и если нужно, то его инициализация / Checking the disk and, if necessary, it will be initialized */
BYTE fs_readdir();                               /* Прочитать содежимое папки в DIRENTRY / Read folder contents in DIRENTRY */
BYTE fs_delete();                                /* Удалить файл или папку, имя в buf / Delete file or folder, name in buf */
BYTE fs_open0(BYTE what);                        /* Открыть/создать файл или папку, имя в buf. Open/create file or foder, name in buf */
BYTE fs_move0();                                 /* Переместить файл/папку. Move file or folder, source file must been opened, destination name in buf */
BYTE fs_move(const char* from, const char* to);  /* Переместить файл/папку. Move file or folder. */
BYTE fs_lseek(DWORD);                            /* Установить указатель чтения/записи файла, увеличивает размер файла. / Set file pointer, enlarge file size. */
BYTE fs_read0(BYTE* ptr, WORD len);              /* Прочитать из файла. НЕЛЬЗЯ ВЫХОДИТЬ ЗА ПРЕДЕЛЫ ФАЙЛА! / Read from the file, DO NOT COME OUT FILE */
BYTE fs_read(BYTE* ptr, WORD len, WORD* readed); /* Прочитать из файла. / Read from the file. */
BYTE fs_write(CONST BYTE* ptr, WORD len);        /* Записать в файл, увеличивает размер файла. / Записать в файл, enlarge file size. */
BYTE fs_write_eof();                             /* Установить конец файла, уменьшает размер файла. / Set end of file, reduces file size */

#ifndef FS_DISABLE_SWAP
void fs_swap();                                  /* Переключится на второй файл / Switch to the second file */
#endif

#ifndef FS_DISABLE_GETFREESPACE
BYTE fs_getfree();                               /* Свободное место на диске в мегабайтах / Free disk space in megabytes */
#endif

DWORD fs_gettotal();                             /* Размер диска в мегабайтах / The size of the disk in megabytes */

/* Функции ниже позволяют использовать для записи буфер (buf).
*  The functions below can be used to write the buffer (buf).
*
*  fs_file.wtotal = размер данных для записи;
*  while(fs_file.wtotal) {
*    fs_write_start();
*    Копируем fs_file_wlen байт в fs_file_wbuf
*    Ни одна функция ФС не должеа выпонятсья между ними
*    fs_write_end();
*  }
*
* See the source of fs_write
* Или смотри исходники fs_write
*/

#define fs_file_wbuf   (buf + (((WORD)fs_file.ptr) & 511))
#define fs_file_wlen   (*(WORD*)&fs_tmp)

BYTE fs_write_start(); 
BYTE fs_write_end();

/* Derived from the function fs_open0 */
/* Производные от функции fs_open0 */

BYTE fs_open();                                  /* Открыть файл */
BYTE fs_opendir();                               /* Открыть папку */
#define fs_openany()    fs_open0(OPENED_NONE)    /* Открыть файл или папку */
#define fs_create()     fs_open0(OPENED_FILE)    /* Создать файл */
#define fs_createdir()  fs_open0(OPENED_DIR)     /* Создать папку */

/* Values fs_opened */
/* Значения fs_opened */

#define OPENED_NONE    0
#define OPENED_FILE    1
#define OPENED_DIR     2

/* File attributes */
/* Атрибуты файлов */

#define	AM_RDO         0x01  /* Read only */
#define	AM_HID         0x02  /* Hidden */
#define	AM_SYS         0x04  /* System */
#define	AM_VOL         0x08  /* Volume label */
#define AM_DIR         0x10  /* Directory */
#define AM_ARC         0x20  /* Archive */

/* Fodler descriptor (FS_DIRENTRY) */
/* Описатель (FS_DIRENTRY) */

#define	DIR_Name       0
#define	DIR_Attr       11
#define	DIR_NTres      12
#define	DIR_CrtTime    14
#define	DIR_CrtDate    16
#define	DIR_FstClusHI  20
#define	DIR_WrtTime    22
#define	DIR_WrtDate    24
#define	DIR_FstClusLO  26
#define	DIR_FileSize   28

#endif