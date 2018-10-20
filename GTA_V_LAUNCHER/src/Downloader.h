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
		void addErrorCode(int code);
		bool isErrorCode(int code) const;
		void removeErrorCode(int code);
	private:
		QNetworkRequest *m_request;
		QSet<int> m_error_codes;
	public slots:
		void fileDownloadedSlot(QNetworkReply*);
		void downloadProgressSlot(qint64 read, qint64 total);
	signals:
		void downloaded(QByteArray res);
		void error();
		void downloadProgress(qint64 read, qint64 total);
};

#endif // DOWNLOADER_H
