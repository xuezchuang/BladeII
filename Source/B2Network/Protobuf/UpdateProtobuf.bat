@echo off
set BUILD_EXE_PATH=..\..\..\..\Engine\Source\ThirdParty\ProtoBuf\bin\protoc.exe
set BUILD_DST_DIR=.\CompiledProtoFiles
set BUILD_TEMP_FILE=__tmp.tmp
set SRC_DIR=.\
set DST_DIR=..\..\..\Build\ProtocolHashes
set DST_INI_DIR=..\..\..\Config

REM check *.proto files exist if not, exit
if not EXIST %SRC_DIR%\\MasterData.proto if not EXIST %SRC_DIR%\\CommonData.proto if not EXIST %SRC_DIR%\\SessionProtocol.proto if not EXIST %SRC_DIR%\\ChannelProtocol.proto GOTO :eof

REM ***********************************************************************************************
REM checkout proto files
REM ***********************************************************************************************
REM p4 edit -c default %BUILD_DST_DIR%\*.*

REM ***********************************************************************************************
REM build proto files
REM ***********************************************************************************************

dir /b %SRC_DIR%\\*.proto > %BUILD_TEMP_FILE%

for /f "tokens=*" %%i in ('type %BUILD_TEMP_FILE%') do (
	call %BUILD_EXE_PATH% -I=%SRC_DIR% --cpp_out=%BUILD_DST_DIR% %SRC_DIR%/%%i
)

del %BUILD_TEMP_FILE%


REM ***********************************************************************************************
REM create hash files
REM ***********************************************************************************************

call %SRC_DIR%\\Hash.bat %SRC_DIR% %DST_DIR% %DST_INI_DIR%

del %SRC_DIR%\\*.sha1

REM ***********************************************************************************************
REM revert unchanged files
REM ***********************************************************************************************
REM p4 revert -c default -a