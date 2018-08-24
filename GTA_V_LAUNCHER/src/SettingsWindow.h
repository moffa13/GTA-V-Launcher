#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QObject>
#include <QDialog>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>

class SettingsWindow : public QDialog
{
	Q_OBJECT

	public:
		explicit SettingsWindow(QWidget *parent = 0);
	private:
		void init();
		void setButtons();
		void connectAll();
		QGroupBox *m_scriptHookVGroupBox;
		QPushButton *m_checkForUpdatesSoftware;
		QPushButton *m_openGTAVGameDirectory;
		QPushButton *m_changeGTAVGameDirectory;
		QPushButton *m_forceGTAQuitButton;
		QVBoxLayout *m_scripthookVLayout;
		QCheckBox *m_startCrackedCheckBox;
		QCheckBox *m_exitLauncherAfterGameStart;
		QVBoxLayout *m_categoriesLayout;
	protected:
		void hideEvent(QHideEvent *e) override;
	signals:

	public slots:
		void openGTAVGameDirectorySlot() const;
		void changeGTAVGameDirectorySlot() const;
		void forceKillGTASlot() const;
		void launchGTAVMethodSlot(int state);
};

#endif // SETTINGSWINDOW_H
