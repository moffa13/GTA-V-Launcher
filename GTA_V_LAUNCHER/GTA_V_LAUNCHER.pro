QT += widgets network xml

RC_FILE = GTA_V_LAUNCHER.rc

TRANSLATIONS = translates/fr.ts

INCLUDEPATH += src ../../libs

VPATH += src ../../libs

LIBS += -lVersion

SOURCES += \
	Start.cpp \
	Window.cpp \
	MainWindow.cpp \
	ChooseModsWindow.cpp \
	Utilities.cpp \
	SettingsWindow.cpp \
	Downloader.cpp \
	Version.cpp \
	InstallModWindow.cpp \
	src/QCheckableFileSystemModel.cpp \
    src/QFileSystemModelDirectorySortProxy.cpp

HEADERS += \
	Window.h \
	MainWindow.h \
	ChooseModsWindow.h \
	Utilities.h \
	SettingsWindow.h \
	Downloader.h \
	Version.h \
	InstallModWindow.h \
	src/QCheckableFileSystemModel.h \
    src/QFileSystemModelDirectorySortProxy.h

RESOURCES += \
	GTA_V_LAUNCHER.qrc


VERSION = 1.0.8
DEFINES += APP_VERSION_COMMA=1,0,8
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

FORMS += \
	src/MainWindow.ui \
	src/ChooseModsWindow.ui \
	src/InstallModWindow.ui
