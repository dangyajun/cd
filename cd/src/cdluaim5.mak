PROJNAME = cd
LIBNAME = cdluaim

OPT = YES

DEF_FILE = cdluaim5.def
SRCDIR = lua5
SRC = cdluaim5.c

ifdef USE_LUA53
  LIBNAME := $(LIBNAME)53
else
ifdef USE_LUA52
  LIBNAME := $(LIBNAME)52
else
  USE_LUA51 = Yes
  LIBNAME := $(LIBNAME)51
endif
endif

USE_CDLUA = YES
USE_IMLUA = YES
# To not link with the Lua dynamic library in UNIX
NO_LUALINK = Yes
# To use a subfolder with the Lua version for binaries
LUAMOD_DIR = Yes
CD = ..

ifneq ($(findstring MacOS, $(TEC_UNAME)), )
  USE_IM = YES
  USE_CD = YES
  USE_IMLUA:=
  USE_CDLUA:=
  INCLUDES += ../include $(IM)/include
  LDIR = ../lib/$(TEC_UNAME) $(IM)/lib/$(TEC_UNAME)
endif
