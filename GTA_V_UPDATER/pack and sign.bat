
set x86PATH=x86\release\GTA_V_UPDATER.exe

..\share\upx.exe --best "%x86PATH%"

set /p passwrd="Password ? "

"C:\Program Files (x86)\Windows Kits\10\bin\x64\signtool.exe" ^
sign /v /f ..\share\cert\moffa.pfx /p %passwrd% /d "GTA V Launcher" /t http://timestamp.verisign.com/scripts/timstamp.dll "%x86PATH%"

copy "%x86PATH%" "updater.exe"