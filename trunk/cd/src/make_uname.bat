@echo off
REM This builds all the libraries of the folder for 1 uname

call tecmake %1 "MF=cd_pdflib" %2 %3 %4 %5 %6 %7 %8

call tecmake %1 %2 %3 %4 %5 %6 %7 %8

call tecmake %1 "MF=cdpdf" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdgl" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdim" %2 %3 %4 %5 %6 %7 %8
  
REM Building for the default (USE_LUA51) 
REM or for the defined at the environment

call tecmake %1 "MF=cdlua5" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdluapdf5" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdluagl5" %2 %3 %4 %5 %6 %7 %8
call tecmake %1 "MF=cdluaim5" %2 %3 %4 %5 %6 %7 %8

REM call tecmake %1 "MF=cdlua5" "USE_LUA51=Yes" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluapdf5" "USE_LUA51=Yes" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluagl5" "USE_LUA51=Yes" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluaim5" "USE_LUA51=Yes" %2 %3 %4 %5 %6 %7 %8

REM call tecmake %1 "MF=cdlua5" "USE_LUA52=Yes" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluapdf5" "USE_LUA52=Yes" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluagl5" "USE_LUA52=Yes" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluaim5" "USE_LUA52=Yes" %2 %3 %4 %5 %6 %7 %8

REM call tecmake %1 "MF=cdlua5" "USE_LUA53=Yes" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluapdf5" "USE_LUA53=Yes" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluagl5" "USE_LUA53=Yes" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluaim5" "USE_LUA53=Yes" %2 %3 %4 %5 %6 %7 %8

REM GDI+ and Cairo are NOT available in some compilers
REM so this may result in errors, just ignore them
call tecmake %1 "MF=cdcontextplus" %2 %3 %4 %5 %6
call tecmake %1 "MF=cdluacontextplus5" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluacontextplus5" "USE_LUA51=Yes" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluacontextplus5" "USE_LUA52=Yes" %2 %3 %4 %5 %6 %7 %8
REM call tecmake %1 "MF=cdluacontextplus5" "USE_LUA53=Yes" %2 %3 %4 %5 %6 %7 %8
