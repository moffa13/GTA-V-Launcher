#include "MainWindow.h"
#include "Utilities.h"
#include <QtDebug>
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include <QSettings>
#include <QIcon>
#include <QNetworkAccessManager>
#include <QDesktopServices>
#include <QDomDocument>
#include <QProcessEnvironment>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMenu>
#include <QThread>
#include <QPushButton>
#include "Version.h"
#include "TranslatorAliases.h"
#include "ui_MainWindow.h"

QString MainWindow::m_gtaDirectoryStr = "";
QString MainWindow::m_disabledModsDirectoryStr = "";

MainWindow::MainWindow(QWidget* parent) : QMainWindow{parent}, ui(new Ui::MainWindow) {
	Q_UNUSED(parent);
	ui->setupUi(this);
}

MainWindow::~MainWindow(){
	delete ui;
}

void MainWindow::init(){

	if(checkOS()){
		setTitle();
		setFavicon();
		setBackground();
		setButtons();
		connectAll();
		show();
		showThanksMessage();
		if(!getGTAExecutable()){
			closeApp();
		}else{
			setGtaVersion();
		}

		if(Utilities::loadFromConfig("General", "shouldCheckForUpdatesWhenLauncherStarts", true).toBool()){
			getLauncherVersion();
			getGtaVersionThrewInternet(true);
		}else{
			getGtaVersionThrewInternet(false); // Fills m_lastOfficialGTAVersion
		}
	}
}

void MainWindow::setTitle(){
	setWindowTitle(tr("GTA V Launcher V") + qApp->applicationVersion());
}

void MainWindow::changeEvent(QEvent *event){
	if(event->type() == QEvent::LanguageChange){
		ui->retranslateUi(this);
		setGtaVersion();
		setTitle();
	} else if(event->type() == QEvent::LocaleChange && TranslatorAliases::getLoadedLanguage() == "default"){
		TranslatorAliases::loadSystemLanguage();
	}
}

void MainWindow::showThanksMessage(){
	int currVersion = static_cast<int>(Version{qApp->applicationVersion()}.getVersionInt());
	if(Utilities::loadFromConfig("General", "lastVersionLauncher", 0).toInt() < currVersion){
		Utilities::setToConfig("General", QMap<QString, QVariant>{{"lastVersionLauncher", currVersion}});
		QDesktopServices::openUrl(QUrl{"https://moffa13.com/"});
	}
}

void MainWindow::uninstallLauncherSlot(bool shouldCloseApp, bool confirmation){
	int resp = confirmation ? QMessageBox::question(this, tr("Uninstall"), tr("Are you sure you do want to uninstall this launcher ? This will re-enable all your mods"))
							: QMessageBox::Yes;
	if(resp == QMessageBox::Yes){
		addScriptHookVDinput();
		ChooseModsWindow::enableAllMods();
		QDir{m_gtaDirectoryStr}.rmdir("disabledMods");
		QDir{m_gtaDirectoryStr + "/installMod"}.removeRecursively();
		Utilities::clearConfig("General");
		Utilities::clearConfig("EnabledMods");
		Utilities::clearConfig("DisabledMods");
		QMessageBox::information(this, tr("Launcher uninstalled"), tr("Successfully uninstalled"), QMessageBox::Ok);
		if(shouldCloseApp) closeApp();
	}
}


QString MainWindow::findGamePath(){
	QStringList possiblesGamePaths;
	possiblesGamePaths << "LNK%SYSTEMDRIVE%\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Rockstar Games\\Grand Theft Auto V";
	possiblesGamePaths << "%SYSTEMDRIVE%\\Program Files\\Rockstar Games\\Grand Theft Auto V";
	possiblesGamePaths << "%SYSTEMDRIVE%\\Program Files (x86)\\Steam\\steamapps\\common\\Grand Theft Auto V";
	possiblesGamePaths << "LNK" + QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first();

	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

	// Iterate over the possibles paths
	for(QString& path : possiblesGamePaths){
		bool isLink = false;
		if(path.left(3) == "LNK"){
			isLink = true;
			path.replace(0, 3, "");
		}
		QDir d{path.replace("%SYSTEMDRIVE%", env.value("SYSTEMDRIVE"))};
		auto list = d.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotAndDotDot);
		for(auto const& e : list){
				QString foundPath = isLink ? e.canonicalFilePath() : e.absoluteFilePath();
				QString parent = QFileInfo(foundPath).absolutePath();
				if(foundPath.right(12) == "PlayGTAV.exe" || foundPath.right(8) == "GTA5.exe") return parent;
		}
	}
	return QString();
}

/**
 * @brief MainWindow::isSteamVersion
 * @return true or false if it's gta's steam version
 */
bool MainWindow::isSteamVersion() const{
	QFileInfo f(this->m_gtaDirectoryStr + "/steam_api.dll");
	QFileInfo f2(this->m_gtaDirectoryStr + "/steam_api64.dll");
	return f.exists() || f2.exists();
}

/**
 * Fetches latest gta version on Rockstar Games servers and call downloadFinishedSlot with the xml as QByteArray
 * @brief MainWindow::getGtaVersionThrewInternet
 */
void MainWindow::getGtaVersionThrewInternet(bool shouldUpdate, bool warnUpToDate){
	if(m_updCheckScriptHookV) return;
	m_updCheckScriptHookV = true;

	m_checkGtaVersion = new Downloader("http://patches.rockstargames.com/prod/gtav/versioning.xml");
	QObject::connect(m_checkGtaVersion, &Downloader::downloaded, [this, shouldUpdate, warnUpToDate](QByteArray const& resp){
		downloadFinishedSlot(resp, shouldUpdate, warnUpToDate);
	});
	QObject::connect(m_checkGtaVersion, &Downloader::error, [this](){
		m_updCheckScriptHookV = false;
		m_checkGtaVersion->deleteLater();
	});
	m_checkGtaVersion->download();
}

void MainWindow::startGTANoUpdate(){

	startGtaWithModsSlot(false, false);

	while(Utilities::checkProcessRunning("GTAVLauncher.exe") == nullptr)
		qApp->processEvents();

	QFile file{
		QString{"%1/%2"}
		.arg(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).takeFirst())
		.arg("Rockstar Games/Social Club/Profiles/signintransfer.dat")
	};

	while(!file.exists())
		qApp->processEvents();

	QThread::msleep(12000);

	system("taskkill /F /IM GTAVLauncher.exe");

	while(Utilities::checkProcessRunning("GTAVLauncher.exe") != nullptr)
		qApp->processEvents();

	startGtaWithModsSlot(true, false);

}

bool MainWindow::checkForUpdateCompatibility(){
	auto scriptHookVVersion{getScriptHookVVersion()};

	// No Mod loader then it's compatible with the game
	if(scriptHookVVersion == Version{})
		return true;

	if(scriptHookVVersion < m_lastOfficialGTAVersion){ // GTA released a new version
		if(scriptHookVVersion == Utilities::getFileVersion(m_gtaDirectoryStr + "/GTA5.exe")){ // User has not updated yet
			if(!isSteamVersion()){
				int resp = QMessageBox::critical(
					this,
					tr("Incompatible Version, start game and block updates ?"),
					tr("GTA V recently updated so when your game will update, you probably won't be able to start it again."
					   " First, try to update ScriptHookV by clicking Cancel then go to Settings -> Check for ScriptHookV updates. If you tried without success "
					   "(No ScriptHookV release yet), click yes. It will start the game and will block GTA from updating."
					   " If you still want to start the game, click No"),
					QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
				);
				if(resp == QMessageBox::Yes){
					startGTANoUpdate();
					return false; // Block updates
				}else if(resp == QMessageBox::Cancel){
					return false; // Do nothing
				}
			}else{
				int resp = QMessageBox::critical(
					this,
					tr("Incompatible Version"),
					tr("GTA V recently updated so when your game will update, you probably won't be able to start it again using mods."
						" To avoid this, please start steam in offline mode then click ok"),
					QMessageBox::Ok | QMessageBox::Cancel
				);
				if(resp == QMessageBox::Ok){
					startGtaWithModsSlot(false, false, true);
				}else return false; // Do nothing

			}
		}else{ // Game already updated, there's nothing we can do
			int resp = QMessageBox::critical(
				this,
				tr("Incompatible Version, start game without mods ?"),
				tr("GTA V recently updated so as your game. You probably won't be able to start it again using mods."
					" Would you want to start the game without mods ?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
			);
			if(resp == QMessageBox::Yes){
				removeScriptHookVDinput();
				startGtaWithModsSlot(false, false, false);
			}else if(resp == QMessageBox::Cancel){
				return false; // Do nothing
			}
		}
		return true; // Normal exec
	}
	return true; // Everything is ok, normal exec

}

/**
 * Fetches latest launcher version on gta5-mods.com and call gotLauncherVersionSlot with the html as QByteArray
 * @brief MainWindow::getGtaVersionThrewInternet
 */
void MainWindow::getLauncherVersion(bool warnUpToDate){
	if(m_updCheckLauncher) return;
	m_updCheckLauncher = true;

	m_checkLauncherVersion = new Downloader("https://www.gta5-mods.com/tools/gta-v-launcher");
	QObject::connect(m_checkLauncherVersion, &Downloader::downloaded, [this, warnUpToDate](QByteArray const& resp){
		gotLauncherVersionSlot(resp, warnUpToDate);
	});
	QObject::connect(m_checkLauncherVersion, &Downloader::error, [this](){
		m_updCheckLauncher = false;
		m_checkLauncherVersion->deleteLater();
	});
	m_checkLauncherVersion->download();
}

void MainWindow::gotLauncherVersionSlot(QByteArray resp, bool warnUpToDate){
	if(resp.isEmpty()){
		qCritical() << "Error launcher update website returned empty response";
	}
	QRegExp version{"<span class=\"version\">([0-9]+\\.[0-9]+\\.[0-9]+)<\\/span>"};
	version.indexIn(resp);
	Version internet{version.cap(1)};
	if(internet > qApp->applicationVersion()){
		int resp = QMessageBox::information(
			this,
			tr("Update"),
			tr("There is a newer update of the launcher (V %1), would you like to be redirected to the website ?")
				.arg(QString{internet.getVersionStr().c_str()}),
			QMessageBox::Yes | QMessageBox::No
		);

		if(resp == QMessageBox::Yes){
			QDesktopServices::openUrl(QUrl{"https://www.gta5-mods.com/tools/gta-v-launcher"});
		}
	}else if(warnUpToDate){
		QMessageBox::information(this, tr("Up-to-date"), tr("You have the latest version of the launcher (V %1)").arg(qApp->applicationVersion()));
	}

	m_updCheckLauncher = false;
	m_checkLauncherVersion->deleteLater();
}

bool MainWindow::checkOS(){
	int OS = QSysInfo::windowsVersion();
	if(OS == QSysInfo::WV_None){
		QMessageBox::critical(this, tr("Error"), tr("This launcher is actually just made for windows"), QMessageBox::Ok);
		closeApp();
		return false;
	}
	return true;
}

void MainWindow::setBackground(){
	QPalette palette;
	QPixmap background(":/images/background.jpg");
	palette.setBrush(QPalette::Background, background);
	setPalette(palette);
}

void MainWindow::closeApp(){
	QTimer::singleShot(0, this, SLOT(closeAppSlot()));
}

bool MainWindow::getGTAExecutable(){
	// Check if already in config
	QString exe = Utilities::loadFromConfig("General", "exe").toString();
	bool alreadyFromConfig = false;
	bool letUserSelect = false;
	if(exe.isEmpty() || !QFile(exe + "/GTA5.exe").exists()){
		// Try to find the exe
		exe = findGamePath();

		// Could find it automatically
		if(!exe.isEmpty()){
			int rep = QMessageBox::information(
				this,
				tr("Found GTA V"),
				tr("We found GTA V in \"%1\", is this the right path ?").arg(exe),
				QMessageBox::Yes | QMessageBox::No
			);

			if(rep != QMessageBox::Yes){
				letUserSelect = true;
			}
		}
	}else
		alreadyFromConfig = true;

	if(!exe.isEmpty() && !letUserSelect){
		if(!alreadyFromConfig)
			Utilities::setToConfig("General", QMap<QString, QVariant>{{"exe", exe}});

		setRelativeDirs(exe);
		setGtaVersion();
		return true;
	}else{

		bool fileExists = false;
		QString dir;

		do {
			dir = QFileDialog::getExistingDirectory(
				this,
				tr("Select your GTA V Path"),
				QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first(),
				QFileDialog::ShowDirsOnly
			);
			fileExists = QFile(dir + "/GTA5.exe").exists();
			if(!fileExists){
				int rep = QMessageBox::critical(
					this,
					tr("Wrong path"),
					tr("This does not seem to be your GTA V folder, please select another one"),
					QMessageBox::Yes | QMessageBox::No
				);
				if(rep == QMessageBox::No){
					return false;
				}
			}
		} while(!fileExists);

		Utilities::setToConfig("General", QMap<QString, QVariant>{{"exe", dir}});
		setRelativeDirs(dir);
		setGtaVersion();
		return true;
	}
}

/**
 * Set the gta folder & disabledMods folder from base
 * @brief MainWindow::setRelativeDirs
 * @param base
 */
void MainWindow::setRelativeDirs(QString const& base){
	m_gtaDirectoryStr = base;
	m_disabledModsDirectoryStr = base + "/disabledMods";
	QDir(MainWindow::m_disabledModsDirectoryStr).mkdir(".");
}

void MainWindow::setGtaVersion(){
	ui->gtaVersionLabel->setAlignment(Qt::AlignRight);
	ui->gtaVersionLabel->setText("GTA V " + QString{getGtaVersion().getVersionStr().c_str()});
}

Version MainWindow::getGtaVersion() const{
	return Utilities::getFileVersion(m_gtaDirectoryStr + "/GTA5.exe");
}

void MainWindow::setFavicon(){
	qApp->setWindowIcon(QIcon(":/images/favicon.png"));
}

void MainWindow::setButtons(){

	QString css("QPushButton{"
					"background-color: #1C1C1C;"
					"color: #EAEAEA;"
					"border: 1px solid #35393b;"
					"font-size: 15px;"
				"border-radius: 2px;"
					"font-family: Consolas, Arial; "
				"}"

				"QPushButton:hover{"
					"border: 2px solid #cecece;"
				"}");


	ui->playOnline->setStyleSheet(css);
	ui->playOnline->update();

	ui->playMods->setStyleSheet(css);
	ui->playMods->setContextMenuPolicy(Qt::CustomContextMenu);

	ui->chooseMods->setStyleSheet(css);

	QPalette byPalette = ui->byLabel->palette();
	byPalette.setColor(QPalette::WindowText, QColor(Qt::white));
	ui->byLabel->setFont(QFont("Arial", 10));
	ui->byLabel->setPalette(byPalette);

	ui->openOptionsButton->setText(tr("Settings"));
	ui->openOptionsButton->setStyleSheet(css);

}

void MainWindow::connectAll(){
	connect(ui->playOnline, SIGNAL(clicked()), this, SLOT(startGtaOnlineSlot()));
	connect(ui->playMods, SIGNAL(clicked()), this, SLOT(startGtaWithModsSlot()));
	connect(ui->chooseMods, SIGNAL(clicked()), this, SLOT(showChooseModsWindowSlot()));
	connect(ui->openOptionsButton, SIGNAL(clicked()), this, SLOT(showSettingsWindowSlot()));
	connect(ui->playMods, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPlayContextualMenuSlot(QPoint)));
}

void MainWindow::closeEvent(QCloseEvent *event){
	event->accept();
	closeApp();
}

/**
 * Removes the main scripts which disables mods and prevents from being banned
 * Returns false if one of the dlls have not been deleted
 * @brief MainWindow::removeScriptHookVDinput
 */
bool MainWindow::removeScriptHookVDinput(bool permanent){
	bool okDelete = true;

	if(permanent || Utilities::copy(m_gtaDirectoryStr + "/dinput8.dll", m_disabledModsDirectoryStr + "/dinput8.dll"))
		okDelete &= QFile::remove(m_gtaDirectoryStr + "/dinput8.dll");
	if(permanent || Utilities::copy(m_gtaDirectoryStr + "/ScriptHookV.dll", m_disabledModsDirectoryStr + "/ScriptHookV.dll"))
		okDelete &= QFile::remove(m_gtaDirectoryStr + "/ScriptHookV.dll");

	return okDelete;
}

/**
 * Does the opposite of MainWindow::removeScriptHookVDinput
 * @brief MainWindow::addScriptHookVDinput
 */
void MainWindow::addScriptHookVDinput(){
	if(Utilities::copy(m_disabledModsDirectoryStr + "/dinput8.dll", m_gtaDirectoryStr + "/dinput8.dll"))
	QFile::remove(m_disabledModsDirectoryStr + "/dinput8.dll");
	if(Utilities::copy(m_disabledModsDirectoryStr + "/ScriptHookV.dll", m_gtaDirectoryStr + "/ScriptHookV.dll"))
	QFile::remove(m_disabledModsDirectoryStr+ "/ScriptHookV.dll");
}

bool MainWindow::checkNeedSteamAndOk(){
	bool ok = !isSteamVersion() || Utilities::checkProcessRunning("Steam.exe") != nullptr;
	if(!ok){
		QMessageBox::critical(this, tr("Error"), tr("Please start steam, log in and try again"));
	}
	return ok;
}

void MainWindow::closeAppSlot(){
	qApp->exit(0);
}

void MainWindow::setGTAHighPriority(){
	Utilities::setProcessPriority("GTA5.exe", 0x80); // 0x80 for high priority
}

void MainWindow::startGtaArgsSlot(QStringList args){
	QProcess m_gtaProcess;
	bool cracked = Utilities::launcherCracked();
	if(cracked){
		m_gtaProcess.setWorkingDirectory(m_gtaDirectoryStr);
		m_gtaProcess.startDetached(m_gtaDirectoryStr + "/Launcher.exe", args);
	}else{
		if(isSteamVersion()){
			QStringList newArgs("/c");
			newArgs << "start steam://rungameid/271590 " + args.join(" ");
			m_gtaProcess.startDetached("cmd", newArgs);
		}else{
			m_gtaProcess.startDetached(m_gtaDirectoryStr + "/GTAVLauncher.exe", args);
		}
	}
	if(Utilities::loadFromConfig("General", "shouldSetGTAProcessAsHighPriority", true).toBool()){
		while(Utilities::checkProcessRunning("GTA5.exe") == nullptr)
			QApplication::processEvents();
		setGTAHighPriority();
	}
	if(Utilities::loadFromConfig("General", "shouldExitLauncherAfterGameStart", true).toBool()){
		closeApp();
	}
}

bool MainWindow::checkGtaAlreadyStarted(bool showMessage){
	bool launcherRunning = Utilities::checkProcessRunning("GTAVLauncher.exe") != nullptr;
	bool gameRunning = Utilities::checkProcessRunning("GTA5.exe") != nullptr;

	if(showMessage && (gameRunning || launcherRunning)) QMessageBox::information(this, tr("GTA V Running"), tr("GTA V is already running"), QMessageBox::Ok);
	return gameRunning || launcherRunning;
}

void MainWindow::startGtaOnlineSlot(){
	if(Utilities::launcherCracked()){
		QMessageBox::critical(this, tr("Error"), tr("You are using a cracked version, online mode is not available"));
		return;
	}
	if(checkGtaAlreadyStarted()) return;
	if(!checkNeedSteamAndOk()) return;
	ChooseModsWindow::disableAllMods();
	if(!removeScriptHookVDinput()){
		int resp = QMessageBox::critical(
			this,
			tr("Error"),
			tr("Could not remove ScriptHookV, this might be dangerous in online mode, would you still like to play ?"),
			QMessageBox::Yes | QMessageBox::No
		);
		if(resp == QMessageBox::No) return;
	}
	if(!checkForUpdateCompatibility()) return;
	QStringList args;
	args << "-StraightIntoFreemode";
	startGtaArgsSlot(args);
}

void MainWindow::startGtaWithModsSlot(bool offlineMode, bool checkForCompatibility, bool addScriptHook){
	if(checkGtaAlreadyStarted()) return;
	if(!checkNeedSteamAndOk()) return;
	if(addScriptHook){
		addScriptHookVDinput();
		ChooseModsWindow::enableOldConfig();
	}
	if(checkForCompatibility && !checkForUpdateCompatibility()) return;
	QStringList args;
	if(offlineMode){
		args << "-scOfflineOnly";
	}
	startGtaArgsSlot(args);
}

void MainWindow::showChooseModsWindowSlot(){
	m_chooseModsWindow = new ChooseModsWindow(this);
	m_chooseModsWindow->exec();
}

Version MainWindow::getScriptHookVVersion(){
	Version scriptHookVVersion = Utilities::getFileVersion(MainWindow::m_gtaDirectoryStr + "/ScriptHookV.dll");
	Version scriptHookVVersionD = Utilities::getFileVersion(MainWindow::m_disabledModsDirectoryStr + "/ScriptHookV.dll");
	return scriptHookVVersion.getVersionInt() != 0 ? scriptHookVVersion : scriptHookVVersionD;
}

void MainWindow::downloadFinishedSlot(QByteArray resp, bool askForUpdate, bool warnUpToDate){
	QDomDocument dom("GTAV");
	dom.setContent(resp);
	QDomElement versioning = dom.documentElement();
	QDomNode versioningNode = versioning.lastChild();
	QDomElement build = versioningNode.toElement();
	QDomNode buildNode = build.elementsByTagName("Game").at(0);
	// Last version on GTA's servers
	m_lastOfficialGTAVersion = buildNode.toElement().attribute("version", "0.0.0.0");
	Version scriptHookVVersion{getScriptHookVVersion()};

	// ScriptHookV is outdated compared to last GTA version on Rockstar servers
	// Not necessarily from current gta version
	if(askForUpdate && scriptHookVVersion < m_lastOfficialGTAVersion){

		QString lastOfficialScriptHookVUrl = QString{"http://www.dev-c.com/files/ScriptHookV_%1.zip"}.arg(QString{m_lastOfficialGTAVersion.getVersionStr().c_str()});
		QString currentGtaScriptHookVUrl = QString{"http://www.dev-c.com/files/ScriptHookV_%1.zip"}.arg(QString{getGtaVersion().getVersionStr().c_str()});

		Downloader *downloader = new Downloader{lastOfficialScriptHookVUrl};
		downloader->addErrorCode(302); // Server does not respond with a normal 404
		downloader->addRawHeader("Referer", "http://www.dev-c.com/gtav/scripthookv/"); // We need this referer in order to access the file

		connect(downloader, &Downloader::downloaded, [this, downloader, scriptHookVVersion, lastOfficialScriptHookVUrl](QByteArray const &resp){

			// First try to check if we get a 302
			// If we reach here, it means the file is available
			if(downloader->isHeadMode()){
				if(downloader->getUrl() == lastOfficialScriptHookVUrl){
					Utilities::setToConfig("General", QMap<QString, QVariant>{{"lastGtaScriptHookVUnavailableWarning", true}});
					int rep;
					if(scriptHookVVersion.getVersionInt() == 0){ // ScriptHookV is not present
						rep = QMessageBox::information(
							this,
							tr("ScriptHookV is missing"),
							tr("To be able to run your mods, you need ScriptHookV. Would you like to install it (V %1) ?")
								.arg(QString(m_lastOfficialGTAVersion.getVersionStr().c_str())),
							QMessageBox::Yes | QMessageBox::No
						);
					}else{
						rep = QMessageBox::information(
							this,
							tr("ScriptHookV out-of-date"),
							tr("Your ScriptHookV version is apparently out-of-date, would you like to update it (V %1) ?")
								.arg(QString(m_lastOfficialGTAVersion.getVersionStr().c_str())),
							QMessageBox::Yes | QMessageBox::No
						);
					}
					if(rep == QMessageBox::Yes){
						downloader->download();
						return;
					}
				}else{
					downloader->download();
					return;
				}


				downloader->deleteLater();
			}

			while(checkGtaAlreadyStarted(false)){
				int rep = QMessageBox::information(this, tr("Update"), tr("Please close GTA V in order to update ScriptHookV"), QMessageBox::Ok | QMessageBox::No);
				if(rep == QMessageBox::No){
					downloader->deleteLater();
					return;
				}
			}

			// Delete ScriptHookV & dinput8 from system (disabled or not)
			addScriptHookVDinput();
			removeScriptHookVDinput(true);

			// Copy unzip program
			QFile::copy(":/unzip.exe", m_gtaDirectoryStr + "/unzip.exe");
			// Remove old ScriptHookV
			QFile::remove("ScriptHookV.zip");
			// Create the new ScriptHookV zip file and write in it
			QFile zip{m_gtaDirectoryStr + "/ScriptHookV.zip"};
			zip.open(QFile::WriteOnly);
			zip.write(resp);
			zip.close();
			// Unzip only ScriptHookV.dll & dinput8.dll
			QProcess process;
			process.setWorkingDirectory(m_gtaDirectoryStr);
			process.start(m_gtaDirectoryStr + "/unzip.exe",
					QStringList() << "-j" << "-o" << "ScriptHookV.zip" << "-x" << "bin/NativeTrainer.asi" << "www.dev-c.com.url" << "readme.txt"
			);
			process.waitForFinished();
			// Remove unzip program & the ScriptHookV zip file to clean the folder
			zip.remove();
			QFile{m_gtaDirectoryStr + "/unzip.exe"}.remove();
			QMessageBox::information(this, tr("Update"), tr("ScriptHookV has successfully been updated/installed !"), QMessageBox::Ok);
			downloader->deleteLater();
		});
		connect(downloader, &Downloader::error, [this, downloader, scriptHookVVersion, currentGtaScriptHookVUrl](){
			if(downloader->isHeadMode()){

				if(Utilities::loadFromConfig("General", "lastGtaScriptHookVUnavailableWarning", true).toBool() || scriptHookVVersion.getVersionInt() == 0){
					QMessageBox::critical(this,
						tr("ScriptHookV Not Found"),
						tr("Could not find ScriptHookV in http://www.dev-c.com, maybe they still don't have released a fix for the new version."
						   " Maybe you will encounter issues when starting the newly updated game.")
					);
					Utilities::setToConfig("General", QMap<QString, QVariant>{{"lastGtaScriptHookVUnavailableWarning", false}});
				}

				if(scriptHookVVersion.getVersionInt() == 0){
					int resp = QMessageBox::critical(this,
						tr("ScriptHookV Not Found"),
						tr("Would you like to download the %1 version of ScriptHookV instead ?").arg(QString{getGtaVersion().getVersionStr().c_str()}),
						QMessageBox::Yes | QMessageBox::No
					);
					if(resp == QMessageBox::Yes){
						downloader->setUrl(currentGtaScriptHookVUrl);
						downloader->head();
						return;
					}
				}
			}else{
				QMessageBox::critical(this,
					tr("Error downloading ScriptHookV"),
					tr("An Error occured while downloading ScriptHookV, please retry")
				);
			}
			downloader->deleteLater();
		});
		downloader->head();

	}else if(warnUpToDate){
		QMessageBox::information(
			this,
			tr("Up-to-date"),
			tr("You have the latest version of ScriptHookV (V %1)").arg(QString{scriptHookVVersion.getVersionStr().c_str()})
		);
	}
	m_updCheckScriptHookV = false;
	m_checkGtaVersion->deleteLater();
}

void MainWindow::showSettingsWindowSlot(){
	m_settingsWindow = new SettingsWindow(this);
	m_settingsWindow->exec();
}

void MainWindow::showPlayContextualMenuSlot(const QPoint &pos){
	QMenu menu;
	QPoint globalPos = ui->playMods->mapToGlobal(pos);
	QAction *act = new QAction(tr("Play offline"), &menu);
	QObject::connect(act, &QAction::triggered, [this](){
		startGtaWithModsSlot(true);
	});
	menu.addAction(act);
	menu.exec(globalPos);
	delete act;
}
