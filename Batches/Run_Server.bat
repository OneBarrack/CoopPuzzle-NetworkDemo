@echo off
set BIN=D:\UnrealProjects\ARPG_ContentDemo\Packaged\WindowsServer\ARPG_ContentDemo\Binaries\Win64
set MAP=/Game/ThirdPerson/Maps/ThirdPersonMap
set PORT=7777

"%BIN%\ARPG_ContentDemoServer.exe" "%MAP%" -log -port=%PORT% -unattended -abslog
