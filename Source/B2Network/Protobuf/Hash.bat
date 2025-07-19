@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

set tmpfile=sha1.tmp
set TARGET_FILE=Hashes.txt
set TARGET_INI=DefaultB2ProtoHash.ini
set SRC_DIR=%1
set DST_DIR=%2
set DST_INI_DIR=%3
set key_value=

REM ***********************************************************************************************
REM create hash files
REM ***********************************************************************************************

REM check *.proto files exist if not, exit
if not EXIST %SRC_DIR%\\MasterData.proto if not EXIST %SRC_DIR%\\CommonData.proto if not EXIST %SRC_DIR%\\SessionProtocol.proto if not EXIST %SRC_DIR%\\ChannelProtocol.proto GOTO :eof

REM convert *.proto files to unix style
dos2unix.exe %SRC_DIR%\\MasterData.proto
dos2unix.exe %SRC_DIR%\\CommonData.proto
dos2unix.exe %SRC_DIR%\\SessionProtocol.proto
dos2unix.exe %SRC_DIR%\\ChannelProtocol.proto

REM generate hash from *.proto files
CertUtil -hashfile %SRC_DIR%\\MasterData.proto SHA1 > %SRC_DIR%\\MasterData.sha1
CertUtil -hashfile %SRC_DIR%\\CommonData.proto SHA1 > %SRC_DIR%\\CommonData.sha1
CertUtil -hashfile %SRC_DIR%\\SessionProtocol.proto SHA1 > %SRC_DIR%\\SessionProtocol.sha1
CertUtil -hashfile %SRC_DIR%\\ChannelProtocol.proto SHA1 > %SRC_DIR%\\ChannelProtocol.sha1

REM remove decorators from hash files
dir /b %SRC_DIR%\\*.sha1 > %tmpfile%

for /f "tokens=*" %%i in ('type %tmpfile%') do (
	call %SRC_DIR%\\ExtractSHA1.bat %SRC_DIR%\\%%i
)

del %tmpfile%

REM ***********************************************************************************************
REM checkout targetfile
REM ***********************************************************************************************
REM p4 edit -c default %DST_DIR%\%TARGET_FILE%
REM p4 edit -c default %DST_INI_DIR%\%TARGET_INI%

REM ***********************************************************************************************
REM update ini file 
REM ***********************************************************************************************

REM add hash sections to ini
echo [ProtocolHash] > %DST_INI_DIR%\\%TARGET_INI%
REM dir /b %SRC_DIR%\\*.sha1 > %tmpfile%
echo MasterData.sha1 > %tmpfile%
echo CommonData.sha1 >> %tmpfile%
echo SessionProtocol.sha1 >> %tmpfile%
echo ChannelProtocol.sha1 >> %tmpfile%

for /f "tokens=1,2 delims=." %%i in ('type %tmpfile%') do ( 
	set /p key_value=<%SRC_DIR%\\%%i.%%j & echo %%iHash=!key_value!>>%DST_INI_DIR%\\%TARGET_INI%
)

del %tmpfile%

REM ***********************************************************************************************
REM update targetfile
REM ***********************************************************************************************
for /f "delims=" %%x in ('type MasterData.sha1') do (
	SET key_value=%%x
)
SET "key_value=%key_value: =%"
echo %key_value%>%DST_DIR%\\%TARGET_FILE%

for /f "delims=" %%x in ('type CommonData.sha1') do (
	SET key_value=%%x
)
SET "key_value=%key_value: =%"
echo %key_value%>>%DST_DIR%\\%TARGET_FILE%

for /f "delims=" %%x in ('type SessionProtocol.sha1') do (
	SET key_value=%%x
)
SET "key_value=%key_value: =%"
echo %key_value%>>%DST_DIR%\\%TARGET_FILE%

for /f "delims=" %%x in ('type ChannelProtocol.sha1') do (
	SET key_value=%%x
)
SET "key_value=%key_value: =%"
echo %key_value%>>%DST_DIR%\\%TARGET_FILE%
