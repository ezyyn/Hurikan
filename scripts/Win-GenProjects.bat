@echo off
pushd ..\
call vendor\bin\Premake\premake5.exe vs2019
popd
PAUSE