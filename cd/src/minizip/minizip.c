/*
minizip.c
Version 1.1, February 14h, 2010
sample part of the MiniZip project - ( http://www.winimage.com/zLibDll/minizip.html )

Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

Modifications of Unzip for Zip64
Copyright (C) 2007-2008 Even Rouault

Modifications for Zip64 support on both zip and unzip
Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )
*/


#if (!defined(_WIN32)) && (!defined(WIN32)) && (!defined(__APPLE__))
#ifndef __USE_FILE_OFFSET64
#define __USE_FILE_OFFSET64
#endif
#ifndef __USE_LARGEFILE64
#define __USE_LARGEFILE64
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif
#ifndef _FILE_OFFSET_BIT
#define _FILE_OFFSET_BIT 64
#endif
#endif

#if defined __APPLE__ || defined USE_FILE32API
// In darwin and perhaps other BSD variants off_t is a 64 bit value, hence no need for specific 64 bit functions
#define FOPEN_FUNC(filename, mode) fopen(filename, mode)
#define FTELLO_FUNC(stream) ftell(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseek(stream, offset, origin)
#else
#define FOPEN_FUNC(filename, mode) fopen64(filename, mode)
#define FTELLO_FUNC(stream) ftello64(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko64(stream, offset, origin)
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#ifdef _WIN32
# include <windows.h>
# include <direct.h>
# include <io.h>
#else
# include <unistd.h>
# include <utime.h>
# include <sys/types.h>
# include <sys/stat.h>
#endif

#include "zip.h"

#define WRITEBUFFERSIZE (16384)
#define MAXFILENAME (256)

#ifdef _WIN32
uLong filetime(f, tmzip, dt)
char *f;                /* name of file to get info on */
tm_zip *tmzip;             /* return value: access, modific. and creation times */
uLong *dt;             /* dostime */
{
  int ret = 0;
  {
    FILETIME ftLocal;
    HANDLE hFind;
    WIN32_FIND_DATAA ff32;

    hFind = FindFirstFileA(f, &ff32);
    if (hFind != INVALID_HANDLE_VALUE)
    {
      FileTimeToLocalFileTime(&(ff32.ftLastWriteTime), &ftLocal);
      FileTimeToDosDateTime(&ftLocal, ((LPWORD)dt) + 1, ((LPWORD)dt) + 0);
      FindClose(hFind);
      ret = 1;
    }
  }
  return ret;
}
#else
#ifdef unix || __APPLE__
uLong filetime(f, tmzip, dt)
char *f;               /* name of file to get info on */
tm_zip *tmzip;         /* return value: access, modific. and creation times */
uLong *dt;             /* dostime */
{
  int ret=0;
  struct stat s;        /* results of stat() */
  struct tm* filedate;
  time_t tm_t=0;

  if (strcmp(f,"-")!=0)
  {
    char name[MAXFILENAME+1];
    int len = strlen(f);
    if (len > MAXFILENAME)
      len = MAXFILENAME;

    strncpy(name, f,MAXFILENAME-1);
    /* strncpy doesnt append the trailing NULL, of the string is too long. */
    name[ MAXFILENAME ] = '\0';

    if (name[len - 1] == '/')
      name[len - 1] = '\0';
    /* not all systems allow stat'ing a file with / appended */
    if (stat(name,&s)==0)
    {
      tm_t = s.st_mtime;
      ret = 1;
    }
  }
  filedate = localtime(&tm_t);

  tmzip->tm_sec  = filedate->tm_sec;
  tmzip->tm_min  = filedate->tm_min;
  tmzip->tm_hour = filedate->tm_hour;
  tmzip->tm_mday = filedate->tm_mday;
  tmzip->tm_mon  = filedate->tm_mon ;
  tmzip->tm_year = filedate->tm_year;

  return ret;
}
#else
uLong filetime(f, tmzip, dt)
char *f;                /* name of file to get info on */
tm_zip *tmzip;             /* return value: access, modific. and creation times */
uLong *dt;             /* dostime */
{
  return 0;
}
#endif
#endif

static int isLargeFile(const char* filename)
{
  int largeFile = 0;
  ZPOS64_T pos = 0;
  FILE* pFile = FOPEN_FUNC(filename, "rb");

  if (pFile != NULL)
  {
    int n = FSEEKO_FUNC(pFile, 0, SEEK_END);
    pos = FTELLO_FUNC(pFile);
    if (pos >= 0xffffffff)
      largeFile = 1;

    fclose(pFile);
  }

  return largeFile;
}

int minizip(const char *filename, const char *temDir, const char **files, int nFiles)
{
  int i;
  int opt_overwrite = 0;
  int opt_compress_level = Z_DEFAULT_COMPRESSION;
  int opt_exclude_path = 0;
  int zipfilenamearg = 0;
  char filename_try[MAXFILENAME + 16];
  int zipok;
  int err = 0;
  int size_buf = 0;
  void* buf = NULL;

  opt_overwrite = 1;
  opt_compress_level = 0;
  opt_exclude_path = 1;

  size_buf = WRITEBUFFERSIZE;

  buf = (void*)malloc(size_buf);
  if (buf == NULL)
    return ZIP_INTERNALERROR;

  int dot_found = 0;

  zipok = 1;
  strncpy(filename_try, filename, MAXFILENAME - 1);
  /* strncpy doesnt append the trailing NULL, of the string is too long. */
  filename_try[MAXFILENAME] = '\0';

  zipFile zf;
  int errclose;
  zf = zipOpen64(filename_try, (opt_overwrite == 2) ? 2 : 0);

  if (zf == NULL)
    err = ZIP_ERRNO;

  for (i = 0; (i < nFiles) && (err == ZIP_OK); i++)
  {
    FILE * fin;
    int size_read;
    const char *savefilenameinzip = files[i];
    const char filenameinzip[MAXFILENAME];
    zip_fileinfo zi;
    unsigned long crcFile = 0;
    int zip64 = 0;

    strcpy(filenameinzip, temDir);
    strcat(filenameinzip, "\\");
    strcat(filenameinzip, savefilenameinzip);

    zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
      zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
    zi.dosDate = 0;
    zi.internal_fa = 0;
    zi.external_fa = 0;
    filetime(filenameinzip, &zi.tmz_date, &zi.dosDate);

    zip64 = isLargeFile(filenameinzip);

    /* The path name saved, should not include a leading slash. */
    /*if it did, windows/xp and dynazip couldn't read the zip file. */
    while (savefilenameinzip[0] == '\\' || savefilenameinzip[0] == '/')
    {
      savefilenameinzip++;
    }

    /**/
    err = zipOpenNewFileInZip3_64(zf, savefilenameinzip, &zi,
                                  NULL, 0, NULL, 0, NULL /* comment*/,
                                  (opt_compress_level != 0) ? Z_DEFLATED : 0,
                                  opt_compress_level, 0,
                                  -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                                  NULL, crcFile, zip64);

    if (err == ZIP_OK)
    {
      fin = FOPEN_FUNC(filenameinzip, "rb");
      if (fin == NULL)
        err = ZIP_ERRNO;
    }

    if (err == ZIP_OK)
    do
    {
      err = ZIP_OK;
      size_read = (int)fread(buf, 1, size_buf, fin);
      if (size_read < size_buf)
      if (feof(fin) == 0)
        err = ZIP_ERRNO;

      if (size_read>0)
        err = zipWriteInFileInZip(zf, buf, size_read);
    } while ((err == ZIP_OK) && (size_read>0));

    if (fin)
      fclose(fin);

    if (err < 0)
      err = ZIP_ERRNO;
    else
      err = zipCloseFileInZip(zf);
  }
  zipClose(zf, NULL);

  free(buf);
  return err;
}
