#include "SettingsWindow.h"
#include "MainWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include "Utilities.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint){
	setButtons();
	connectAll();
	init();
}

void SettingsWindow::init(){
	setWindowTitle(tr("Settings"));
	setFixedSize(sizeHint());
}

void SettingsWindow::setButtons(){
	m_scriptHookVGroupBox = new QGroupBox("GTA V Launcher", this);
	m_scripthookVLayout = new QHBoxLayout(m_scriptHookVGroupBox);

	m_checkForUpdatesSoftware = new QPushButton(tr("Check for updates of ScriptHookV"), this);
	m_startCrackedCheckBox = new QCheckBox(tr("Launch from crack"), this);
	m_forceGTAQuitButton = new QPushButton(tr("Force kill GTA V Process"), this);
	m_openGTAVGameDirectory = new QPushButton(tr("Open GTA V Game Directory"), this);
	m_changeGTAVGameDirectory = new QPushButton(tr("Change GTA V Game Directory"), this);


	bool cracked = Utilities::launcherCracked();
	m_startCrackedCheckBox->setChecked(cracked);
	m_startCrackedCheckBox->setCheckState(cracked ? Qt::Checked : Qt::Unchecked);

	m_scripthookVLayout->addWidget(m_startCrackedCheckBox);
	m_scripthookVLayout->addWidget(m_checkForUpdatesSoftware);

	m_scriptHookVGroupBox->setLayout(m_scripthookVLayout);

	m_categoriesLayout = new QVBoxLayout(this);
	m_categoriesLayout->addWidget(m_scriptHookVGroupBox);
	m_categoriesLayout->addWidget(m_openGTAVGameDirectory);
	m_categoriesLayout->addWidget(m_changeGTAVGameDirectory);
	m_categoriesLayout->addWidget(m_forceGTAQuitButton);
	setLayout(m_categoriesLayout);
}

void SettingsWindow::openGTAVGameDirectorySlot() const{
	QDesktopServices::openUrl(QUrl::fromLocalFile(MainWindow::m_gtaDirectoryStr));
}

void SettingsWindow::changeGTAVGameDirectorySlot() const{
	MainWindow *parent = qobject_cast<MainWindow*>(this->parentWidget());
	Utilities::setToConfig("General", QMap<QString, QVariant>{{"exe", ""}});
	if(!parent->getGTAExecutable()){
		parent->closeApp();
	}
}

void SettingsWindow::forceKillGTASlot() const{
	QProcess::execute("taskkill /im GTA5.exe /f");
	QProcess::execute("taskkill /im GTAVLauncher.exe /f");
}

void SettingsWindow::connectAll(){
	QObject::connect(m_checkForUpdatesSoftware, &QPushButton::clicked, [this](){
		MainWindow *parent = qobject_cast<MainWindow*>(this->parentWidget());
		parent->getSoftwareUpdates();
	});
	connect(m_forceGTAQuitButton, SIGNAL(clicked(bool)), this, SLOT(forceKillGTASlot()));
	connect(m_openGTAVGameDirectory, SIGNAL(clicked(bool)), this, SLOT(openGTAVGameDirectorySlot()));
	connect(m_changeGTAVGameDirectory, SIGNAL(clicked(bool)), this, SLOT(changeGTAVGameDirectorySlot()));

	QObject::connect(m_startCrackedCheckBox, SIGNAL(stateChanged(int)), this, SLOT(launchGTAVMethodSlot(int)));
}

void SettingsWindow::hideEvent(QHideEvent *e){
	e->accept();
	deleteLater();
}

void SettingsWindow::launchGTAVMethodSlot(int state){
	qDebug() << state;
	QMap<QString, QVariant> checkBox;
	if(state == 0){ // Unchecked
		state = false;
	}else if(state == 2){ // Checked
		QFile f(MainWindow::m_gtaDirectoryStr + "/Launcher.exe");
		bool exist = false;
		state = true;
		do{
			exist = f.exists();
			if(!exist){
				int resp = QMessageBox::critical(this, "Not found", tr("Can't find 3DM Launcher, please be sure that the file is in the GTA V root folder and is named \"Launcher.exe\""
									  "Also, make sure that \"3dmgame.dll\" and \"3dmgame.ini\" are present"), QMessageBox::Ok | QMessageBox::Cancel);
				if(resp == QMessageBox::Cancel){
					m_startCrackedCheckBox->setChecked(false);
					m_startCrackedCheckBox->setCheckState(Qt::Unchecked);
					state = false;
					break;
				}
			}
		}while(!exist);
	}
	checkBox["LauncherCrack"] = QVariant(state);
	Utilities::setToConfig("General", checkBox);
}

