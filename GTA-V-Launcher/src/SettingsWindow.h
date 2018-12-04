#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QObject>
#include "SelfDeleteDialog.h"
#include "ThreadedProgressBar.h"
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>

class MainWindow;

class SettingsWindow : public SelfDeleteDialog
{
	Q_OBJECT

	public:
		explicit SettingsWindow(QWidget *parent = nullptr);
	private:
		void init();
		void setButtons();
		void connectAll();
		MainWindow *getParent() const;
		ThreadedProgressBar *m_filesCheckProgress;
		QGroupBox *m_scriptHookVGroupBox;
		QPushButton *m_checkForLauncherUpdates;
		QPushButton *m_checkForUpdatesSoftware;
		QPushButton *m_openGTAVGameDirectory;
		QPushButton *m_changeGTAVGameDirectory;
		QPushButton *m_forceGTAQuitButton;
		QPushButton *m_uninstallLauncher;
		QPushButton *m_checkFilesIntegrity;
		QVBoxLayout *m_scripthookVLayout;
		QCheckBox *m_startCrackedCheckBox;
		QCheckBox *m_exitLauncherAfterGameStart;
		QCheckBox *m_checkForUpdatesWhenLauncherStarts;
		QVBoxLayout *m_categoriesLayout;
	public slots:
		void openGTAVGameDirectorySlot() const;
		void changeGTAVGameDirectorySlot() const;
		void forceKillGTASlot() const;
		void launchGTAVMethodSlot(int state);
	private slots:
		void checkSoftwareUpdatesSlot(bool warnUpToDate) const;
		void checkLauncherUpdatesSlot() const;
};

#endif // SETTINGSWINDOW_H
