QT += widgets network xml concurrent

CONFIG += c++11
QMAKE_CXXFLAGS += -MD
QMAKE_CXXFLAGS_RELEASE = -O2

RC_FILE = GTA-V-Launcher.rc

TRANSLATIONS = translates/fr.ts

INCLUDEPATH += src include

VPATH += src ../../libs

LIBS += -Lsrc/lib/openssl -llibcrypto -llibssl -lVersion

SOURCES += \
	Start.cpp \
	MainWindow.cpp \
	ChooseModsWindow.cpp \
	Utilities.cpp \
	SettingsWindow.cpp \
	Downloader.cpp \
	Version.cpp \
	InstallModWindow.cpp \
	QCheckableFileSystemModel.cpp \
	QFileSystemModelDirectorySortProxy.cpp \
	src/SelfDeleteDialog.cpp \
	src/MD5Hasher.cpp \
	src/GTAFilesChecker.cpp \
	src/ThreadedProgressBar.cpp

HEADERS += \
	Window.h \
	MainWindow.h \
	ChooseModsWindow.h \
	Utilities.h \
	SettingsWindow.h \
	Downloader.h \
	Version.h \
	InstallModWindow.h \
	QCheckableFileSystemModel.h \
	QFileSystemModelDirectorySortProxy.h \
	src/SelfDeleteDialog.h \
	src/MD5Hasher.h \
	src/GTAFilesChecker.h \
	src/ThreadedProgressBar.h

RESOURCES += \
	GTA-V-Launcher.qrc


VERSION = 1.0.12
DEFINES += APP_VERSION_COMMA=1,0,12
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

FORMS += \
	MainWindow.ui \
	ChooseModsWindow.ui \
	InstallModWindow.ui
