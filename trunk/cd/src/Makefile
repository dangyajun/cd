ifeq ($(OS), Windows_NT)
  TECMAKE_CMD = $(MAKE) --no-print-directory -f ../tecmakewin.mak TEC_UNAME=$(TEC_UNAME)
else
  TECMAKE_CMD = $(MAKE) --no-print-directory -f ../tecmake.mak
endif

.PHONY: do_all cd_zlib cd_freetype cd_ftgl cd cd_pdflib cdpdf cdgl cdcontextplus cdlua5 cdluapdf5 cdluagl5 cdluacontextplus5 cdluaim5
do_all:        cd_zlib cd_freetype cd_ftgl cd cd_pdflib cdpdf cdgl cdcontextplus cdlua5 cdluapdf5 cdluagl5 cdluacontextplus5 cdluaim5

cd_zlib:
	@$(TECMAKE_CMD) MF=cd_zlib
cd_freetype:
	@$(TECMAKE_CMD) MF=cd_freetype
cd_ftgl:
	@$(TECMAKE_CMD) MF=cd_ftgl
cd_pdflib:
	@$(TECMAKE_CMD) MF=cd_pdflib
  
cd:
	@$(TECMAKE_CMD)
  
cdpdf:
	@$(TECMAKE_CMD) MF=cdpdf
cdgl:
	@$(TECMAKE_CMD) MF=cdgl
  
cdcontextplus:
	@$(TECMAKE_CMD) MF=cdcontextplus
  
# Building for the default (USE_LUA51) 
# or for the defined at the environment
  
cdlua5:
	@$(TECMAKE_CMD) MF=cdlua5 
#	@$(TECMAKE_CMD) MF=cdlua5 USE_LUA51=Yes
#	@$(TECMAKE_CMD) MF=cdlua5 USE_LUA52=Yes
#	@$(TECMAKE_CMD) MF=cdlua5 USE_LUA53=Yes
cdluapdf5:
	@$(TECMAKE_CMD) MF=cdluapdf5 
#	@$(TECMAKE_CMD) MF=cdluapdf5 USE_LUA51=Yes
#	@$(TECMAKE_CMD) MF=cdluapdf5 USE_LUA52=Yes
#	@$(TECMAKE_CMD) MF=cdluapdf5 USE_LUA53=Yes
cdluagl5:
	@$(TECMAKE_CMD) MF=cdluagl5 
#	@$(TECMAKE_CMD) MF=cdluagl5 USE_LUA51=Yes
#	@$(TECMAKE_CMD) MF=cdluagl5 USE_LUA52=Yes
#	@$(TECMAKE_CMD) MF=cdluagl5 USE_LUA53=Yes
cdluacontextplus5:
	@$(TECMAKE_CMD) MF=cdluacontextplus5 
#	@$(TECMAKE_CMD) MF=cdluacontextplus5 USE_LUA51=Yes
#	@$(TECMAKE_CMD) MF=cdluacontextplus5 USE_LUA52=Yes
#	@$(TECMAKE_CMD) MF=cdluacontextplus5 USE_LUA53=Yes
cdluaim5:
	@$(TECMAKE_CMD) MF=cdluaim5 
#	@$(TECMAKE_CMD) MF=cdluaim5 USE_LUA51=Yes
#	@$(TECMAKE_CMD) MF=cdluaim5 USE_LUA52=Yes
#	@$(TECMAKE_CMD) MF=cdluaim5 USE_LUA53=Yes
