QT += widgets network

INCLUDEPATH += src

RC_FILE = GTA_V_UPDATER.rc

RESOURCES += \
	GTA_V_UPDATER.qrc

TRANSLATIONS += translates/fr.ts

VERSION = 1.0.0

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

VPATH = src ../../libs

INCLUDEPATH = ../../libs

SOURCES += \
	Source.cpp \
	Window.cpp \
	Mainwindow.cpp \
	network/Downloader.cpp

HEADERS += \
	Window.h \
	Mainwindow.h \
	network/Downloader.h
