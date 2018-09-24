QT += widgets network xml

RC_FILE = GTA_V_LAUNCHER.rc

TRANSLATIONS = translates/fr.ts

INCLUDEPATH += src ../../libs

VPATH += src ../../libs

LIBS += -lVersion

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
    src/SelfDeleteDialog.cpp

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
    src/SelfDeleteDialog.h

RESOURCES += \
	GTA_V_LAUNCHER.qrc


VERSION = 1.0.8
DEFINES += APP_VERSION_COMMA=1,0,8
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

FORMS += \
	MainWindow.ui \
	ChooseModsWindow.ui \
	InstallModWindow.ui
