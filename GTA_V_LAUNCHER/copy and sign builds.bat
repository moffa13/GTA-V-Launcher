@ECHO OFF

del Release.rar

set x86PATH=ToRelease\Launcher\GTA V Launcher.exe
::set x64PATH=ToRelease\Launcher\GTA V Launcher.exe

copy "x86\release\GTA_V_Launcher.exe" "%x86PATH%"
::copy "x64\release\GTA_V_Launcher.exe" "%x64PATH%"

..\share\upx.exe --best "%x86PATH%"

set /p passwrd="Password ? "

"C:\Program Files (x86)\Windows Kits\10\bin\x64\signtool.exe" ^
sign /v /f ..\share\cert\moffa.pfx /p %passwrd% /d "GTA V Launcher" /t http://timestamp.verisign.com/scripts/timstamp.dll "%x86PATH%"

rar a -ep1 Release.rar ToRelease\Launcher

::CALL clean.bat

pause