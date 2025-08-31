@echo off
set UE=D:\Engine\UnrealEngine-5.5
set PROJ=D:\UnrealProjects\ARPG_ContentDemo\ARPG_ContentDemo.uproject

"%UE%\Engine\Build\BatchFiles\Build.bat" ARPG_ContentDemoEditor Win64 Development -Project="%PROJ%" -WaitMutex || goto :eof
"%UE%\Engine\Build\BatchFiles\Build.bat" ARPG_ContentDemo       Win64 Development -Project="%PROJ%" -WaitMutex || goto :eof
"%UE%\Engine\Build\BatchFiles\Build.bat" ARPG_ContentDemoServer Win64 Development -Project="%PROJ%" -WaitMutex || goto :eof

echo === Build_All DONE ===
pause
