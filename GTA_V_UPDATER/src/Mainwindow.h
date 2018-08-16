#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Window.h"
#include "network/Downloader.h"
#include <QProgressBar>
#include <QLabel>

class MainWindow : public Window
{

	Q_OBJECT

	public:
		MainWindow();
		void init();
	private:
		void closeApp();
		void setComponents();
		bool checkArguments();
		void downloadLauncher();
		QProgressBar *m_progressBar;
		QLabel *m_infoLabel;
		Downloader *m_launcherDownload;
		QString m_versionTxt;
		QNetworkReply *m_download;
		static QString m_thisDir;
	public slots:
		void closeAppSlot();
		void launcherDownloadedSlot(QByteArray resp);
		void launcherErrorDownloadSlot();
		void updateProgressSlot(qint64 read, qint64 total);
};

#endif // MAINWINDOW_H
