PROJNAME = cd
LIBNAME = cdluapdf

OPT = YES

SRCDIR = lua5
SRC = cdluapdf5.c
DEF_FILE = cdluapdf5.def

LIBS = cdpdf

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

# To not link with the Lua dynamic library in UNIX
NO_LUALINK = Yes
# To use a subfolder with the Lua version for binaries
LUAMOD_DIR = Yes
USE_CDLUA = YES
CD = ..

ifneq ($(findstring MacOS, $(TEC_UNAME)), )
  USE_CD = YES
  USE_CDLUA:=
  INCLUDES += ../include
  LDIR = ../lib/$(TEC_UNAME)
endif
