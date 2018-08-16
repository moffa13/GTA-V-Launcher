#include "MainWindow.h"
#include "Utilities.h"
#include <QtDebug>
#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include <QIcon>
#include <QNetworkAccessManager>
#include <QDomDocument>
#include <QMenu>
#include "ui_MainWindow.h"

QString MainWindow::m_gtaDirectoryStr = "M:/GamesNotSteam/GTAV";
QString MainWindow::m_disabledModsDirectoryStr = "M:/GamesNotSteam/GTAV/Launcher/disabledMods";

MainWindow::MainWindow(QWidget* parent) : Window(), ui(new Ui::MainWindow){
	#ifndef QT_DEBUG
		QDir d(qApp->applicationDirPath());
		d.cdUp();
		m_gtaDirectoryStr = d.absolutePath();
		m_disabledModsDirectoryStr = QDir::fromNativeSeparators(qApp->applicationDirPath()+"/disabledMods");
	#endif
	ui->setupUi(this);
}

MainWindow::~MainWindow(){
	delete ui;
}

void MainWindow::init(){

	if(checkOS() && getGTAExecutable() != NULL){
		//setFixedSize(960, 600);
		setWindowTitle("Launcher GTA V "+ qApp->applicationVersion());
		setFavicon();
		setBackground();
		setButtons();
		connectAll();
		show();
		deleteTemp();
		getSoftwareUpdates();
		//getGtaVersionThrewInternet();
	}
}

void MainWindow::deleteTemp(){
	QFile::remove(qApp->applicationDirPath()+"/unrar.exe");
	QFile::remove(qApp->applicationDirPath()+"/updater.exe");
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
void MainWindow::getSoftwareUpdates(bool messageBox){
	m_checkSoftwareUpdates = new Downloader("http://beta.bigcoding.com/GTAVLauncher/getVersionList");
	QObject::connect(m_checkSoftwareUpdates, &Downloader::downloaded, [this, messageBox](QByteArray const &resp){
		checkSoftwareUpdatesSlot(resp, messageBox);
	});
	m_checkSoftwareUpdates->download();
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

QString MainWindow::getGTAExecutable(){
	QString exe = this->m_gtaDirectoryStr + "/GTAVLauncher.exe";
	QFile f(exe);
	if(f.exists()){
		return exe;
	}else{
		QMessageBox::critical(this, tr("Error"), tr("Unable to find GTA launcher, is the launcher in the correct directory ?"), QMessageBox::Ok);
		closeApp();
		return NULL;
	}
}

void MainWindow::setFavicon(){
	qApp->setWindowIcon(QIcon(":/images/favicon.png"));
}

void MainWindow::setButtons(){

	QString css("QPushButton{"
					"background-color: rgb(51, 185, 54);"
					"border:1px solid black;"
					"font-size:15px;"
					"font-family:Consolas, Arial; "
				"}"

				"QPushButton:hover{"
					"border:1px solid #cecece;"
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

int MainWindow::getVersionToInt(QString version){
	return version.replace('.', "").toInt();
}

/**
 * Removes the main scripts which disables mods and prevents to be banned
 * @brief MainWindow::removeScriptHookVDinput
 */
void MainWindow::removeScriptHookVDinput(){
	QFile::copy(m_gtaDirectoryStr + "/dinput8.dll", m_disabledModsDirectoryStr + "/dinput8.dll");
	QFile::remove(m_gtaDirectoryStr + "/dinput8.dll");
	QFile::copy(m_gtaDirectoryStr + "/ScriptHookV.dll", m_disabledModsDirectoryStr + "/ScriptHookV.dll");
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
	QProcess *m_gtaProcess = new QProcess();
	bool cracked = Utilities::launcherCracked();
	if(cracked){
		qDebug() << "Start as cracked";
		m_gtaProcess->setWorkingDirectory(m_gtaDirectoryStr);
		m_gtaProcess->start(m_gtaDirectoryStr + "/Launcher.exe", args);
	}else{
		qDebug() << "Start as official";
		if(isSteamVersion()){
			qDebug() << "Steam version";
			QStringList newArgs("/c");
			newArgs << "start steam://rungameid/271590 " + args.join(" ");
			m_gtaProcess->start("cmd", newArgs);
		}else{
			qDebug() << "Normal version";
			m_gtaProcess->start(m_gtaDirectoryStr + "/GTAVLauncher.exe", args);
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
	args << "-StraightIntoFreemode";
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
	int gameVersionNow = buildNode.toElement().attribute("version", "0.0.0.0").replace('.', "").toInt();
	int userGameVersion = getVersionToInt(Utilities::getFileVersion(MainWindow::m_gtaDirectoryStr+"/GTA5.exe"));
	int scriptHookVVersion = getVersionToInt(Utilities::getFileVersion(MainWindow::m_gtaDirectoryStr+"/ScriptHookV.dll"));
	if(userGameVersion < gameVersionNow && scriptHookVVersion < gameVersionNow){
		QMessageBox::information(this, tr("GTA V out-of-date"), tr("Your GTA V version is apparently out-of-date, would you like to update ScriptHookV ?"), QMessageBox::Yes);
	}else{
		qDebug() << "GTA up-to-date (v"+QVariant(gameVersionNow).toString()+")";
	}
	m_checkGtaVersion->deleteLater();
}

void MainWindow::showSettingsWindowSlot(){
	m_settingsWindow = new SettingsWindow(this);
	m_settingsWindow->exec();
}

void MainWindow::checkSoftwareUpdatesSlot(QByteArray const &resp, bool messageBox){
	QDomDocument q("update");
	q.setContent(resp);
	QDomElement versioning = q.documentElement();
	QDomNode versioningNode = versioning.firstChild();
	QDomElement build = versioningNode.toElement();
	QString newVersionStr = build.attribute("version", "0.0.0");
	int newVersion = getVersionToInt(newVersionStr);
	int version = getVersionToInt(qApp->applicationVersion());
	qDebug() << version << newVersion;
	if(version < newVersion){
		int resp = QMessageBox::information(this, tr("Launcher released"), tr("New version of GTA V Launcher has been released (v%n), would you like to download it ?", "", newVersion),
				QMessageBox::Yes | QMessageBox::No);
		if(resp == QMessageBox::Yes){
			QString updater = qApp->applicationDirPath()+"/updater.exe";
			Downloader *d = new Downloader("http://beta.bigcoding.com/GTAVLauncher/getUpdater");
			d->download();
			QObject::connect(d, &Downloader::downloaded, [this, updater, newVersionStr](QByteArray const &resp){
				QFile q(updater);
				q.open(QIODevice::WriteOnly);
				q.write(resp);
				q.close();
				QProcess *process = new QProcess;
				process->start(updater, QStringList(newVersionStr));
				closeApp();
			});
		}
	}else{
		if(messageBox){
			QMessageBox::information(this, tr("Up-to-date"), tr("You have the last version of the launcher"));
		}
		qDebug() << "Launcher up-to-date (v"+QVariant(newVersion).toString()+")";
	}
	m_checkSoftwareUpdates->deleteLater();
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
