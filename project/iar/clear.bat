::@echo off
rem serching...
rem delete file
for %%b in ("%cd%") do cd /d %%b&for /r %%c in ("Debug","settings") do if exist %%c rmdir /s/q  "%%c"
for /f "delims=" %%i in ('dir /b /a-d /s "*.sfr","*.dep","*.tmp","path.txt","*.tmp.c","*.orig","*.h.bak","*.c.bak","*.sfb"') do del /q "%%i"
rem delete done
pause
