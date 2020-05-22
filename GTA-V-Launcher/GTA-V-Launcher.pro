QT += widgets network xml concurrent

CONFIG += c++11
QMAKE_CXXFLAGS += -MT
QMAKE_CXXFLAGS_RELEASE = -O2

RC_FILE = GTA-V-Launcher.rc

TRANSLATIONS = \
translates/fr.ts \
translates/pt_BR.ts \
translates/ru.ts

INCLUDEPATH += src include

VPATH += src ../../libs

LIBS += -Llib\openssl -llibcrypto -llibssl -lVersion

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
	src/ThreadedProgressBar.cpp \
	src/TranslatorAliases.cpp

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
	src/ThreadedProgressBar.h \
	src/TranslatorAliases.h

RESOURCES += \
	GTA-V-Launcher.qrc


VERSION = 1.0.13
DEFINES += APP_VERSION_COMMA=1,0,13
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

FORMS += \
	MainWindow.ui \
	ChooseModsWindow.ui \
	InstallModWindow.ui
