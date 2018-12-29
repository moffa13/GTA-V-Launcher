#include "SettingsWindow.h"
#include "MainWindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include "GTAFilesChecker.h"
#include "TranslatorAliases.h"
#include "ThreadedProgressBar.h"
#include "Utilities.h"

SettingsWindow::SettingsWindow(QWidget *parent) : SelfDeleteDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint){
	setButtons();
	connectAll();
}

void SettingsWindow::setButtons(){

	m_languageLabel = new QLabel(this);
	m_chooseLanguage = new QComboBox(this);
	m_languageLayout = new QHBoxLayout;
	m_chooseLanguage->addItem(QString(), "default");
	auto languages = TranslatorAliases::getAvailableLanguages();
	for(auto const& language : languages){
		QPixmap icon = QIcon{QString{":/translates/flags/%1.png"}.arg(language.second)}.pixmap(QSize{16, 12});
		m_chooseLanguage->addItem(icon, language.first, language.second);

	}

	m_chooseLanguage->setCurrentIndex(m_chooseLanguage->findData(TranslatorAliases::getLoadedLanguage()));

	m_languageLayout->addWidget(m_languageLabel);
	m_languageLayout->addWidget(m_chooseLanguage, 1);

	m_scriptHookVGroupBox = new QGroupBox(this);
	m_scripthookVLayout = new QVBoxLayout(m_scriptHookVGroupBox);
	m_checkForLauncherUpdates = new QPushButton(this);
	m_deleteAllMods = new QPushButton(this);
	m_gtaHighPriority = new QCheckBox(this);
	m_checkForUpdatesSoftware = new QPushButton(this);
	m_startCrackedCheckBox = new QCheckBox(this);
	m_exitLauncherAfterGameStart = new QCheckBox(this);
	m_checkForUpdatesWhenLauncherStarts = new QCheckBox(this);
	m_forceGTAQuitButton = new QPushButton(this);
	m_openGTAVGameDirectory = new QPushButton(this);
	m_openGTAVGameDirectory->setToolTip(MainWindow::m_gtaDirectoryStr);
	m_changeGTAVGameDirectory = new QPushButton(this);
	m_checkFilesIntegrity = new QPushButton(this);
	m_uninstallLauncher = new QPushButton(this);


	bool cracked = Utilities::launcherCracked();
	m_startCrackedCheckBox->setChecked(cracked);
	m_startCrackedCheckBox->setCheckState(cracked ? Qt::Checked : Qt::Unchecked);

	bool shouldExitLauncherAfterGameStart = Utilities::loadFromConfig("General", "shouldExitLauncherAfterGameStart", true).toBool();
	m_exitLauncherAfterGameStart->setChecked(shouldExitLauncherAfterGameStart);
	m_exitLauncherAfterGameStart->setCheckState(shouldExitLauncherAfterGameStart ? Qt::Checked : Qt::Unchecked);

	bool shouldCheckForUpdatesWhenLauncherStarts = Utilities::loadFromConfig("General", "shouldCheckForUpdatesWhenLauncherStarts", true).toBool();
	m_checkForUpdatesWhenLauncherStarts->setChecked(shouldCheckForUpdatesWhenLauncherStarts);
	m_checkForUpdatesWhenLauncherStarts->setCheckState(shouldCheckForUpdatesWhenLauncherStarts ? Qt::Checked : Qt::Unchecked);

	bool shouldSetGTAProcessAsHighPriority = Utilities::loadFromConfig("General", "shouldSetGTAProcessAsHighPriority", true).toBool();
	m_gtaHighPriority->setChecked(shouldSetGTAProcessAsHighPriority);
	m_gtaHighPriority->setCheckState(shouldSetGTAProcessAsHighPriority ? Qt::Checked : Qt::Unchecked);

	m_scripthookVLayout->addWidget(m_startCrackedCheckBox);
	m_scripthookVLayout->addWidget(m_exitLauncherAfterGameStart);
	m_scripthookVLayout->addWidget(m_checkForUpdatesWhenLauncherStarts);
	m_scripthookVLayout->addWidget(m_gtaHighPriority);
	m_scripthookVLayout->addLayout(m_languageLayout);
	m_scripthookVLayout->addWidget(m_checkForLauncherUpdates);
	m_scripthookVLayout->addWidget(m_checkForUpdatesSoftware);
	m_scripthookVLayout->addWidget(m_openGTAVGameDirectory);
	m_scripthookVLayout->addWidget(m_changeGTAVGameDirectory);
	m_scripthookVLayout->addWidget(m_forceGTAQuitButton);
	m_scripthookVLayout->addWidget(m_checkFilesIntegrity);
	m_scripthookVLayout->addWidget(m_uninstallLauncher);
	m_scripthookVLayout->addWidget(m_deleteAllMods);

	m_scriptHookVGroupBox->setLayout(m_scripthookVLayout);

	m_categoriesLayout = new QVBoxLayout(this);
	m_categoriesLayout->addWidget(m_scriptHookVGroupBox);
	setLayout(m_categoriesLayout);
	retranslateUi();
}

void SettingsWindow::retranslateUi(){
	setWindowTitle(tr("Settings"));
	m_languageLabel->setText(tr("Language : "));
	m_chooseLanguage->setItemText(0, tr("Default system's language"));
	m_scriptHookVGroupBox->setTitle("GTA V Launcher");
	m_checkForLauncherUpdates->setText(tr("Check for launcher updates"));
	m_checkForUpdatesSoftware->setText(tr("Check for ScriptHookV updates"));
	m_startCrackedCheckBox->setText(tr("Launch from crack"));
	m_gtaHighPriority->setText(tr("Start GTA V with high process priority"));
	m_exitLauncherAfterGameStart->setText(tr("Exit launcher after game starts"));
	m_checkForUpdatesWhenLauncherStarts->setText(tr("Check for updates when launcher starts"));
	m_forceGTAQuitButton->setText(tr("Force kill GTA V Process"));
	m_openGTAVGameDirectory->setText(tr("Open GTA V Game Directory"));
	m_changeGTAVGameDirectory->setText(tr("Change GTA V Game Directory"));
	m_checkFilesIntegrity->setText(tr("Check GTA V Files Integrity"));
	m_uninstallLauncher->setText(tr("Uninstall this launcher"));
	m_deleteAllMods->setText(tr("Delete all GTA V mods and clean install"));
	adjustSize();
	setFixedSize(sizeHint());
}

void SettingsWindow::openGTAVGameDirectorySlot() const{
	QDesktopServices::openUrl(QUrl::fromLocalFile(MainWindow::m_gtaDirectoryStr));
}

void SettingsWindow::changeGTAVGameDirectorySlot() const{
	MainWindow *parent = qobject_cast<MainWindow*>(this->parentWidget());
	Utilities::setToConfig("General", QMap<QString, QVariant>{{"exe", ""}});
	if(!parent->getGTAExecutable()){
		parent->closeApp();
	}else
		checkSoftwareUpdatesSlot(false);
}

void SettingsWindow::forceKillGTASlot(){
	QProcess::execute("taskkill /im GTA5.exe /f");
	QProcess::execute("taskkill /im GTAVLauncher.exe /f");
}

void SettingsWindow::connectAll(){
	QObject::connect(m_checkForLauncherUpdates, SIGNAL(clicked(bool)), this, SLOT(checkLauncherUpdatesSlot()));
	QObject::connect(m_checkForUpdatesSoftware, &QPushButton::clicked, this, [this](){
		checkSoftwareUpdatesSlot(true);
	});
	connect(m_forceGTAQuitButton, SIGNAL(clicked(bool)), this, SLOT(forceKillGTASlot()));
	connect(m_openGTAVGameDirectory, SIGNAL(clicked(bool)), this, SLOT(openGTAVGameDirectorySlot()));
	connect(m_changeGTAVGameDirectory, SIGNAL(clicked(bool)), this, SLOT(changeGTAVGameDirectorySlot()));

	connect(m_startCrackedCheckBox, SIGNAL(stateChanged(int)), this, SLOT(launchGTAVMethodSlot(int)));
	connect(m_exitLauncherAfterGameStart, &QCheckBox::stateChanged, [](int state){
		Utilities::setToConfig("General", QMap<QString, QVariant>{{"shouldExitLauncherAfterGameStart", state}});
	});
	connect(m_checkForUpdatesWhenLauncherStarts, &QCheckBox::stateChanged, [](int state){
		Utilities::setToConfig("General", QMap<QString, QVariant>{{"shouldCheckForUpdatesWhenLauncherStarts", state}});
	});
	connect(m_gtaHighPriority, &QCheckBox::stateChanged, [](int state){
		Utilities::setToConfig("General", QMap<QString, QVariant>{{"shouldSetGTAProcessAsHighPriority", state}});
	});

	connect(m_checkFilesIntegrity, &QPushButton::clicked, this, &SettingsWindow::checkFilesIntegritySlot);

	connect(m_chooseLanguage, QOverload<int>::of(&QComboBox::activated), [this](int index){
		if(index == 0)
			TranslatorAliases::loadSystemLanguage();
		else
			TranslatorAliases::loadLanguage(m_chooseLanguage->currentData().toString(), false);
	});

	connect(m_deleteAllMods, &QPushButton::clicked, [this](){
		int resp = QMessageBox::warning(
			this,
			tr("Warning"),
			tr("Are you sure that you do want to remove every mods and restore GTA V to a clean state ? This will also uninstall the launcher."),
			QMessageBox::Yes | QMessageBox::No
		);
		if(resp == QMessageBox::Yes){

			while(getParent()->checkGtaAlreadyStarted(false)){
				int rep = QMessageBox::information(this, tr("Cleaning"), tr("Please close GTA V in order to restore your installation."), QMessageBox::Ok | QMessageBox::No);
				if(rep == QMessageBox::No){
					return;
				}
			}
			GTAFilesChecker checker{MainWindow::m_gtaDirectoryStr};
			bool noError = checker.rootRemoveAllUnofficialFiles();
			if(!noError){
				int resp = QMessageBox::critical(
					this,
					tr("Error"),
					tr("There was an error while removing some files, would you like to continue ?"),
					QMessageBox::Yes | QMessageBox::No
				);
				if(resp == QMessageBox::No){
					return;
				}
			}
			QMessageBox::information(this, tr("Success"), tr("All mods have been removed, now checking integrity."));

			auto conn = std::make_shared<QMetaObject::Connection>();
			auto conn2 = std::make_shared<QMetaObject::Connection>();
			*conn = connect(this, &SettingsWindow::finishedIntegrityCheck, [this, conn, conn2](){
				QObject::disconnect(*conn);
				QObject::disconnect(*conn2);
				getParent()->uninstallLauncherSlot(true, false);
			});
			// If integrity check is canceled, do not uninstall launcher next time you do an integrity check
			*conn2 = connect(this, &SettingsWindow::integrityCheckAborted, [conn, conn2](){
				QObject::disconnect(*conn);
				QObject::disconnect(*conn2);
			});


			checkFilesIntegritySlot();
		}

	});

	connect(m_uninstallLauncher, SIGNAL(clicked(bool)), getParent(), SLOT(uninstallLauncherSlot()));
}

void SettingsWindow::checkFilesIntegritySlot(){
	GTAFilesChecker *checker = new GTAFilesChecker{MainWindow::m_gtaDirectoryStr};
	m_filesCheckProgress = new ThreadedProgressBar{this};
	m_filesCheckProgress->setWindowTitle(tr("Checking GTA V Files ..."));
	m_filesCheckProgress->setMax(checker->getSize());
	m_filesCheckProgress->setFixedSize(QSize(250, 60));
	m_filesCheckProgress->show();
	connect(m_filesCheckProgress, &ThreadedProgressBar::hidden, [checker, this](){
		m_filesCheckProgress = nullptr;
		if(checker->isRunning()){
			Q_EMIT integrityCheckAborted();
			checker->stop();
		}
		checker->deleteLater();
	});

	connect(checker, &GTAFilesChecker::bytesProcessing, [this](quint64 value){
		if(m_filesCheckProgress != nullptr)
			m_filesCheckProgress->add(value);
	});
	connect(checker, &GTAFilesChecker::fileProcessing, [this](QString const& file){
		if(m_filesCheckProgress != nullptr)
			m_filesCheckProgress->setLabel(file);
	});
	connect(checker, &GTAFilesChecker::success, [this, checker](){
		m_filesCheckProgress->hide();
		QMessageBox::information(this, tr("Success"), tr("File checker returned no error."));
		Q_EMIT finishedIntegrityCheck();
		checker->deleteLater();
	});
	connect(checker, &GTAFilesChecker::error, [this, checker](){
		m_filesCheckProgress->hide();
		QMessageBox::critical(this, tr("Checksum error"), tr("There is an error with these files : %1").arg(checker->getErrors().join(", ")));
		if(checker->hasRealCorruptedFiles()){
			int response = QMessageBox::question(
						this,
						tr("Delete corrupted files ?"),
						tr("Do you want to delete the corrupted files so they will be downloaded at the next start of GTA V ?"),
						QMessageBox::Yes | QMessageBox::No
			);
			if(response == QMessageBox::Yes){
				if(checker->deleteCorrupted()){
					QMessageBox::information(this, tr("Success"), tr("Files have been deleted !"));
				}else{
					QMessageBox::critical(this, tr("Error"), tr("Error deleting corrupted files"));
				}
			}
		}
		Q_EMIT finishedIntegrityCheck();
		checker->deleteLater();
	});
	checker->check();
}

MainWindow *SettingsWindow::getParent() const{
	return qobject_cast<MainWindow*>(parentWidget());
}

void SettingsWindow::checkSoftwareUpdatesSlot(bool warnUpToDate) const{
	MainWindow *parent = qobject_cast<MainWindow*>(this->parentWidget());
	parent->getGtaVersionThrewInternet(true, warnUpToDate);
}

void SettingsWindow::checkLauncherUpdatesSlot() const{
	MainWindow *parent = qobject_cast<MainWindow*>(this->parentWidget());
	parent->getLauncherVersion(true);
}

void SettingsWindow::launchGTAVMethodSlot(int state){
	QMap<QString, QVariant> checkBox;
	if(state == Qt::Unchecked){
		state = false;
	}else if(state == Qt::Checked){
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

