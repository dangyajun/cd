PROJNAME = cd
LIBNAME = cdluapdf

OPT = YES

DEFINES = CD_NO_OLD_INTERFACE
SRCDIR = lua5
SRC = cdluapdf5.c
DEF_FILE = cdluapdf5.def

LIBS = cdpdf

ifdef USE_LUA53
  LIBNAME := $(LIBNAME)53
  DEFINES += LUA_COMPAT_MODULE
else
ifdef USE_LUA52
  LIBNAME := $(LIBNAME)52
  DEFINES += LUA_COMPAT_MODULE
else
  USE_LUA51 = Yes
  LIBNAME := $(LIBNAME)51
endif
endif

NO_LUALINK = Yes
USE_CDLUA = YES
CD = ..

ifneq ($(findstring MacOS, $(TEC_UNAME)), )
  USE_CD = YES
  USE_CDLUA:=
  INCLUDES += ../include
  LDIR = ../lib/$(TEC_UNAME)
endif
