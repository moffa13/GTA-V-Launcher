@echo off
SetLocal EnableDelayedExpansion
(set PATH=C:\Users\moffa\Desktop\qt-everywhere-src-5.11.1\qt-everywhere-src-5.11.1\qtbase\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=C:\Users\moffa\Desktop\qt-everywhere-src-5.11.1\qt-everywhere-src-5.11.1\qtbase\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=C:\Users\moffa\Desktop\qt-everywhere-src-5.11.1\qt-everywhere-src-5.11.1\qtbase\plugins
)
C:\Users\moffa\Desktop\qt-everywhere-src-5.11.1\qt-everywhere-src-5.11.1\qtbase\bin\uic.exe %*
EndLocal
