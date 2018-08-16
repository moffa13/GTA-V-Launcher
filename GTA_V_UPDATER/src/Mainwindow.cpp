#include "Mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QStringList>
#include <QTimer>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QProcess>

QString MainWindow::m_thisDir = "M:/Games/GTA V New/Launcher";

MainWindow::MainWindow(){
	#ifndef QT_DEBUG
		QDir d(qApp->applicationDirPath());
		m_thisDir = d.absolutePath();
	#endif
}

void MainWindow::init(){
	if(checkArguments()){
		setWindowTitle(tr("Updater"));
		setFixedSize(400, 60);
		setComponents();
		show();
		QFile::copy(":/unrar.exe", m_thisDir+"/unrar.exe");
		downloadLauncher();
	}else{
		closeApp();
	}
}

void MainWindow::closeApp(){
	QTimer::singleShot(0, this, SLOT(closeAppSlot()));
}

bool MainWindow::checkArguments(){
	QStringList args = qApp->arguments();
	if(args.length() > 1){
		QString arg = qApp->arguments().at(1);
		m_versionTxt = arg;
		return true;
	}else{
		QMessageBox::critical(this, tr("Error"), tr("Error occured, please re-download launcher"));
		return false;
	}
}

void MainWindow::downloadLauncher(){
	m_launcherDownload = new Downloader("http://beta.bigcoding.com/GTAVLauncher/getVersion/"+m_versionTxt);
	m_download = m_launcherDownload->download();

	QObject::connect(m_launcherDownload, SIGNAL(error()), this, SLOT(launcherErrorDownloadSlot()));
	QObject::connect(m_launcherDownload, SIGNAL(downloaded(QByteArray)), this, SLOT(launcherDownloadedSlot(QByteArray)));
	QObject::connect(m_launcherDownload, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateProgressSlot(qint64,qint64)));
}

void MainWindow::setComponents(){
	m_progressBar = new QProgressBar(this);
	m_progressBar->setGeometry(10, 30, 380, 25);

	m_infoLabel = new QLabel(tr("GTA V Launcher is updating ..."), this);
	m_infoLabel->move(10, 10);
}

void MainWindow::closeAppSlot(){
	qApp->exit(0);
}

void MainWindow::launcherDownloadedSlot(QByteArray const &resp){
	const QString launcherRarPath = QDir::fromNativeSeparators(m_thisDir+"/"+m_versionTxt+".rar");
	const QString unrarPath = QDir::fromNativeSeparators(m_thisDir+"/unrar.exe");
	QFile f(launcherRarPath);
	f.open(QIODevice::WriteOnly);
	f.write(resp);
	QProcess *process = new QProcess;
	process->setWorkingDirectory(m_thisDir);
	bool removed;
	QFile f2(m_thisDir+"/GTA V Launcher.exe");
	if(f2.exists()){
		do{
			removed = f2.remove();
			if(!removed){
				QMessageBox::critical(this, tr("Launcher is running"), tr("Please quit the launcher before continue"));
			}
		}while(!removed);
	}
	QStringList args;
	args << "e";
	args << launcherRarPath;
	args << "Launcher\\GTA V Launcher.exe";
	process->start(unrarPath, args);
	void (QProcess::*signal)(int, QProcess::ExitStatus) = &QProcess::finished;
	QObject::connect(process, signal, [process, unrarPath, launcherRarPath, this](){
		QFile::remove(launcherRarPath);
		QFile::remove(unrarPath);
		process->deleteLater();
		QProcess *p2 = new QProcess;
		p2->start(m_thisDir+"/GTA V Launcher.exe", QStringList());
		closeApp();
	});
}

void MainWindow::launcherErrorDownloadSlot(){
	QMessageBox::critical(this, tr("Error"), tr("Launcher can't be downloaded, please re-download launcher"));
	closeApp();
}

void MainWindow::updateProgressSlot(qint64 read, qint64 total){
	m_progressBar->setMaximum(total);
	m_progressBar->setValue(read);
}

