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
#include <QComboBox>

class MainWindow;

class SettingsWindow : public SelfDeleteDialog
{
	Q_OBJECT

	public:
		explicit SettingsWindow(QWidget *parent = nullptr);
	private:
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
		QPushButton *m_deleteAllMods;
		QVBoxLayout *m_scripthookVLayout;
		QCheckBox *m_startCrackedCheckBox;
		QCheckBox *m_gtaHighPriority;
		QCheckBox *m_exitLauncherAfterGameStart;
		QCheckBox *m_checkForUpdatesWhenLauncherStarts;
		QVBoxLayout *m_categoriesLayout;
		QComboBox *m_chooseLanguage;
		QLabel *m_languageLabel;
		QHBoxLayout *m_languageLayout;
	public slots:
		void openGTAVGameDirectorySlot() const;
		void changeGTAVGameDirectorySlot() const;
		static void forceKillGTASlot();
		void launchGTAVMethodSlot(int state);
	private slots:
		void checkSoftwareUpdatesSlot(bool warnUpToDate) const;
		void checkLauncherUpdatesSlot() const;
		void checkFilesIntegritySlot();
	Q_SIGNALS:
		void finishedIntegrityCheck() const;
		void integrityCheckAborted() const;
	protected:
		virtual void retranslateUi() override;
};

#endif // SETTINGSWINDOW_H
