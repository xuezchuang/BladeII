@echo off
set tmpfile=__tmp.tmp
set DST_DIR=..\..\..\..\..\BladeII\Source\B2Network\Protobuf\CompiledProtoFiles
set SRC_DIR=..\..\..\..\..\BladeII\Source\B2Network\Protobuf

dir /b %SRC_DIR%\*.proto > %tmpfile%

for /f "tokens=*" %%i in ('type %tmpfile%') do (
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/%%i
)

del %tmpfile%