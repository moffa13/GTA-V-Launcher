@ECHO OFF

mkdir bundle

set pth=bundle\GTA V Launcher.exe

copy "build\build-GTA-V-Launcher-5_12_x64_static_openssl-Release\release\GTA-V-Launcher.exe" "%pth%"

third-party\upx.exe --best "%pth%"

set /p passwrd="Password ? "

"C:\Program Files (x86)\Windows Kits\10\bin\10.0.16299.0\x64\signtool.exe" ^
sign /v /f cert\moffa.pfx /p %passwrd% /d "GTA V Launcher" /t http://timestamp.verisign.com/scripts/timstamp.dll "%pth%"

third-party\zip.exe -r "bundle\GTA V Launcher.zip" "bundle\GTA V Launcher.exe"

::CALL clean.bat

pause