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
	network/Downloader.cpp \
    src/Version.cpp

HEADERS += \
	Window.h \
	MainWindow.h \
	ChooseModsWindow.h \
	Utilities.h \
	SettingsWindow.h \
	network/Downloader.h \
    src/Version.h

RESOURCES += \
	GTA_V_LAUNCHER.qrc

VERSION = 1.0.7

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

FORMS += \
    MainWindow.ui \
    ChooseModsWindow.ui
