@echo off
set BIN=D:\UnrealProjects\ARPG_ContentDemo\Packaged\Windows\ARPG_ContentDemo\Binaries\Win64
set HOST=127.0.0.1
set PORT=7777
set FPS=90

"%BIN%\ARPG_ContentDemo.exe" %HOST%:%PORT% -log -windowed -ResX=1280 -ResY=720 -ExecCmds="t.IdleWhenNotForeground 0, t.MaxFPS %FPS%"
