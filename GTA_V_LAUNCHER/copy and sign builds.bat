@ECHO OFF

del Release.rar
mkdir ToRelease\Launcher

set x86PATH=ToRelease\Launcher\GTA V Launcher.exe

copy "release\GTA_V_LAUNCHER.exe" "%x86PATH%"

..\share\upx.exe --best "%x86PATH%"

set /p passwrd="Password ? "

"C:\Program Files (x86)\Windows Kits\10\bin\x64\signtool.exe" ^
sign /v /f ..\share\cert\moffa.pfx /p %passwrd% /d "GTA V Launcher" /t http://timestamp.verisign.com/scripts/timstamp.dll "%x86PATH%"

rar a -ep1 Release.rar ToRelease\Launcher

::CALL clean.bat

pause