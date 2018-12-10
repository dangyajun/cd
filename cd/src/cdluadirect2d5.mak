PROJNAME = cd
LIBNAME = cdluadirect2d

OPT = YES

SRCDIR = lua5
SRC = cdluadirect2d5.c
DEF_FILE = cdluadirect2d5.def

LIBS = cddirect2d

ifdef USE_LUA_VERSION
  USE_LUA51:=
  USE_LUA52:=
  USE_LUA53:=
  ifeq ($(USE_LUA_VERSION), 53)
    USE_LUA53:=Yes
  endif
  ifeq ($(USE_LUA_VERSION), 52)
    USE_LUA52:=Yes
  endif
  ifeq ($(USE_LUA_VERSION), 51)
    USE_LUA51:=Yes
  endif
endif

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
