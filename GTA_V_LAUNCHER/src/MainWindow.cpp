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
#include <QDomDocument>
#include <QProcessEnvironment>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMenu>
#include "Version.h"
#include "ui_MainWindow.h"

QString MainWindow::m_gtaDirectoryStr = "";
QString MainWindow::m_disabledModsDirectoryStr = "";

MainWindow::MainWindow(QWidget* parent) : Window(), ui(new Ui::MainWindow){
	ui->setupUi(this);
}

MainWindow::~MainWindow(){
	delete ui;
}

void MainWindow::init(){

	if(checkOS()){
		setWindowTitle("Launcher GTA V "+ qApp->applicationVersion());
		setFavicon();
		setBackground();
		setButtons();
		connectAll();
		show();
		if(!getGTAExecutable()){
			closeApp();
		}
		getSoftwareUpdates();
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
				if(foundPath.right(12) == "PlayGTAV.exe") return parent;
		}
	}
	return QString();
}

/**
 * @brief MainWindow::isSteamVersion
 * @return true or false if it's gta's steam version
 */
bool MainWindow::isSteamVersion(){
	QFileInfo f(this->m_gtaDirectoryStr+"/steam_api.dll");
	QFileInfo f2(this->m_gtaDirectoryStr+"/steam_api64.dll");
	return f.exists() || f2.exists();
}

/**
 * Fetches latest gta version on Rockstar Games servers and call downloadFinishedSlot with the xml as QByteArray
 * @brief MainWindow::getGtaVersionThrewInternet
 */
void MainWindow::getGtaVersionThrewInternet(){
	m_checkGtaVersion = new Downloader("http://patches.rockstargames.com/prod/gtav/versioning.xml");
	QObject::connect(m_checkGtaVersion, SIGNAL(downloaded(QByteArray)), this, SLOT(downloadFinishedSlot(QByteArray)));
	m_checkGtaVersion->download();
}

/**
 * Fetches latest launcher version and calls checkSoftwareUpdatesSlot sending it the current prod version
 * @brief MainWindow::getSoftwareUpdates
 * @param messageBox
 */
void MainWindow::getSoftwareUpdates(){
	getGtaVersionThrewInternet();
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
	if(exe.isEmpty() || !QFile(exe + "/PlayGTAV.exe").exists()){
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
			fileExists = QFile(dir + "/PlayGTAV.exe").exists();
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
}

void MainWindow::setFavicon(){
	qApp->setWindowIcon(QIcon(":/images/favicon.png"));
}

void MainWindow::setButtons(){

	QString css("QPushButton{"
					"background-color: #55955c;"
					"border: 1px solid black;"
					"font-size: 15px;"
					"font-family: Consolas, Arial; "
				"}"

				"QPushButton:hover{"
					"border: 2px solid #cecece;"
				"}");


	ui->playOnline->setText(tr("Play GTA V Online\n (Mods disabled)"));
	ui->playOnline->setStyleSheet(css);
	ui->playOnline->update();

	ui->playMods = new QPushButton(tr("Play GTA V"), this);
	ui->playMods->setStyleSheet(css);
	ui->playMods->setGeometry(481, 438, 200, 80);
	ui->playMods->setContextMenuPolicy(Qt::CustomContextMenu);

	ui->chooseMods = new QPushButton(tr("Choose mods"), this);
	ui->chooseMods->setStyleSheet(css);
	ui->chooseMods->setGeometry(481, 519, 200, 29);

	ui->byLabel = new QLabel(tr("Created By Moffa13 @ moffa13.com"), this);
	QPalette byPalette = ui->byLabel->palette();
	byPalette.setColor(QPalette::WindowText, QColor(Qt::white));
	ui->byLabel->setFont(QFont("Arial", 10));
	ui->byLabel->setPalette(byPalette);
	ui->byLabel->move(10, 580);

	ui->openOptionsButton = new QPushButton(tr("Settings"), this);
	ui->openOptionsButton->setStyleSheet(css);
	ui->openOptionsButton->setGeometry(15, 15, 153, 57);

}

void MainWindow::connectAll(){
	QObject::connect(ui->playOnline, SIGNAL(clicked()), this, SLOT(startGtaOnlineSlot()));
	QObject::connect(ui->playMods, SIGNAL(clicked()), this, SLOT(startGtaWithModsSlot()));
	QObject::connect(ui->chooseMods, SIGNAL(clicked()), this, SLOT(showChooseModsWindowSlot()));
	QObject::connect(ui->openOptionsButton, SIGNAL(clicked()), this, SLOT(showSettingsWindowSlot()));
	QObject::connect(ui->playMods, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPlayContextualMenuSlot(QPoint)));

}

void MainWindow::closeEvent(QCloseEvent *event){
	event->accept();
	closeApp();
}

/**
 * Removes the main scripts which disables mods and prevents to be banned
 * @brief MainWindow::removeScriptHookVDinput
 */
void MainWindow::removeScriptHookVDinput(bool permanent){
	if(!permanent){
		QFile::copy(m_gtaDirectoryStr + "/dinput8.dll", m_disabledModsDirectoryStr + "/dinput8.dll");
		QFile::copy(m_gtaDirectoryStr + "/ScriptHookV.dll", m_disabledModsDirectoryStr + "/ScriptHookV.dll");
	}
	QFile::remove(m_gtaDirectoryStr + "/dinput8.dll");
	QFile::remove(m_gtaDirectoryStr + "/ScriptHookV.dll");
}

/**
 * Does the opposite of MainWindow::removeScriptHookVDinput
 * @brief MainWindow::addScriptHookVDinput
 */
void MainWindow::addScriptHookVDinput(){
	QFile::copy(m_disabledModsDirectoryStr + "/dinput8.dll", m_gtaDirectoryStr + "/dinput8.dll");
	QFile::remove(m_disabledModsDirectoryStr + "/dinput8.dll");
	QFile::copy(m_disabledModsDirectoryStr + "/ScriptHookV.dll", m_gtaDirectoryStr + "/ScriptHookV.dll");
	QFile::remove(m_disabledModsDirectoryStr+ "/ScriptHookV.dll");
}

bool MainWindow::checkNeedSteamAndOk(){
	bool ok = !isSteamVersion() || Utilities::checkProcessRunning("Steam.exe") != NULL;
	if(!ok){
		QMessageBox::critical(this, tr("Error"), tr("Please start steam, log in and try again"));
	}
	return ok;
}

void MainWindow::closeAppSlot(){
	qApp->exit(0);
}

void MainWindow::startGtaArgsSlot(QStringList args){
	QProcess m_gtaProcess;
	bool cracked = Utilities::launcherCracked();
	if(cracked){
		qDebug() << "Start as cracked";
		m_gtaProcess.setWorkingDirectory(m_gtaDirectoryStr);
		m_gtaProcess.startDetached(m_gtaDirectoryStr + "/Launcher.exe", args);
	}else{
		qDebug() << "Start as official";
		if(isSteamVersion()){
			qDebug() << "Steam version";
			QStringList newArgs("/c");
			newArgs << "start steam://rungameid/271590 " + args.join(" ");
			m_gtaProcess.startDetached("cmd", newArgs);
		}else{
			qDebug() << "Normal version";
			m_gtaProcess.startDetached(m_gtaDirectoryStr + "/GTAVLauncher.exe", args);
		}
	}
}

bool MainWindow::checkGtaAlreadyStarted(bool showMessage){
	bool launcherRunning = Utilities::checkProcessRunning("GTAVLauncher.exe") != NULL;
	bool gameRunning = Utilities::checkProcessRunning("GTA5.exe") != NULL;

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
	removeScriptHookVDinput();
	QStringList args;
	args << "-goStraightToMP";
	startGtaArgsSlot(args);
}

void MainWindow::startGtaWithModsSlot(bool offlineMode){
	if(checkGtaAlreadyStarted()) return;
	if(!checkNeedSteamAndOk()) return;
	addScriptHookVDinput();
	ChooseModsWindow::enableOldConfig();
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

void MainWindow::downloadFinishedSlot(QByteArray resp){
	QDomDocument dom("GTAV");
	dom.setContent(resp);
	QDomElement versioning = dom.documentElement();
	QDomNode versioningNode = versioning.lastChild();
	QDomElement build = versioningNode.toElement();
	QDomNode buildNode = build.elementsByTagName("Game").at(0);
	Version gameVersionNow = buildNode.toElement().attribute("version", "0.0.0.0");
	Version scriptHookVVersion = Utilities::getFileVersion(MainWindow::m_gtaDirectoryStr + "/ScriptHookV.dll");
	if(scriptHookVVersion < gameVersionNow){
		int rep = QMessageBox::information(
					this,
					tr("ScriptHookV out-of-date"),
					tr("Your ScriptHookV version is apparently out-of-date or you don't have it, would you like to update/install it (V %1) ?")
						.arg(QString(gameVersionNow.getVersionStr().c_str())),
					QMessageBox::Yes | QMessageBox::No
		);
		if(rep == QMessageBox::Yes){
			Downloader *downloader = new Downloader{"http://www.dev-c.com/files/ScriptHookV_" + QString{gameVersionNow.getVersionStr().c_str()} + ".zip"};
			downloader->addRawHeader("Referer", "http://www.dev-c.com/gtav/scripthookv/");
			connect(downloader, &Downloader::downloaded, [this, downloader](QByteArray const &resp){

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
			connect(downloader, &Downloader::error, [this, downloader](){
				QMessageBox::critical(this, tr("ScriptHookV Not Found"), tr("Could not find ScriptHookV in http://www.dev-c.com"));
				downloader->deleteLater();
			});
			downloader->download();
		}
	}
	m_checkGtaVersion->deleteLater();
}

void MainWindow::showSettingsWindowSlot(){
	m_settingsWindow = new SettingsWindow(this);
	m_settingsWindow->exec();
}

void MainWindow::showPlayContextualMenuSlot(const QPoint &pos){
	QMenu menu;
	QPoint globalPos = ui->playMods->mapToGlobal(pos);
	QAction *act = new QAction(tr("Play offline"), ui->playMods);
	QObject::connect(act, &QAction::triggered, [this](){
		startGtaWithModsSlot(true);
	});
	menu.addAction(act);
	menu.exec(globalPos);
	delete act;
}
