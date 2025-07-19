@echo off
 
set count=1
for /f "tokens=1" %%a in (%1) do (
    call :subroutine %%a %1
)
GOTO :eof
 
:subroutine
 if %count% EQU 2 @echo %1 > %2
 set /a count+=1