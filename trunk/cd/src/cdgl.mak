PROJNAME = cd
LIBNAME = cdgl
OPT = YES

SRC = drv/cdgl.c

INCLUDES = . sim ftgl freetype2 freetype2/include
LIBS = ftgl

USE_OPENGL = Yes
USE_MACOS_OPENGL = Yes
USE_CD = YES
CD = ..

ifneq ($(findstring Win, $(TEC_SYSNAME)), )
  ifeq ($(findstring dll, $(TEC_UNAME)), )
    DEFINES += FTGL_LIBRARY_STATIC
  endif
endif

ifneq ($(findstring MacOS, $(TEC_UNAME)), )
  INCLUDES += $(X11_INC)
  ifdef USE_MACOS_OPENGL
    LFLAGS = -framework OpenGL
    USE_OPENGL :=
  endif
  ifneq ($(TEC_SYSMINOR), 4)
    BUILD_DYLIB=Yes
  endif
  LIBS += iconv
  LDIR += $(GTK)/lib
endif

ifneq ($(findstring cygw, $(TEC_UNAME)), )
  LIBS += iconv
endif
