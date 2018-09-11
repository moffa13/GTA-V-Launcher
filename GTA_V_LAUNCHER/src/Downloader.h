#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QString>
#include <QObject>
#include <QNetworkReply>
#include <QUrl>

class Downloader : public QNetworkAccessManager
{

	Q_OBJECT

	public:
		Downloader(QString url);
		Downloader(QNetworkRequest request);
		~Downloader();
		QNetworkReply *download();
		QNetworkReply *rep;
		void addRawHeader(const QString &field, const QString &value);
	private:
		QNetworkRequest *m_request;
	public slots:
		void fileDownloadedSlot(QNetworkReply*);
		void downloadProgressSlot(qint64 read, qint64 total);
	signals:
		void downloaded(QByteArray res);
		void error();
		void downloadProgress(qint64 read, qint64 total);
};

#endif // DOWNLOADER_H
