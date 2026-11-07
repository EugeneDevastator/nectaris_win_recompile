@echo off
set PATH=C:\msys64\mingw32\bin;%PATH%
cd /d "%~dp0"
gcc.exe -shared -O2 -o winmm.dll proxy.c winmm.def -Wl,--enable-stdcall-fixup -static-libgcc > build.log 2>&1
echo exit=%ERRORLEVEL%
type build.log
