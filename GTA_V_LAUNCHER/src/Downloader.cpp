#include "Downloader.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QObject>
#include <QTimer>

Downloader::Downloader(QString url){
	m_request = new QNetworkRequest(QUrl(url));
	m_error_codes.insert(404);
}

Downloader::Downloader(QNetworkRequest request){
	m_request = new QNetworkRequest(request);
}

Downloader::~Downloader(){
	delete m_request;
}

void Downloader::addRawHeader(QString const& field, QString const& value){
	m_request->setRawHeader(field.toLocal8Bit(), value.toLocal8Bit());
}

void Downloader::addErrorCode(int code){
	m_error_codes.insert(code);
}

bool Downloader::isErrorCode(int code) const{
	return m_error_codes.contains(code);
}

void Downloader::removeErrorCode(int code){
	m_error_codes.remove(code);
}

QNetworkReply* Downloader::download(){
	m_request->setHeader(QNetworkRequest::UserAgentHeader, "GTA V Launcher by Moffa13");
	QNetworkAccessManager *q = new QNetworkAccessManager(this);
	QObject::connect(q, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloadedSlot(QNetworkReply*)));
	rep = q->get(*m_request);
	QObject::connect(rep, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgressSlot(qint64,qint64)));
	return rep;
}

void Downloader::fileDownloadedSlot(QNetworkReply *reply){
	QUrl movedUrl(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl());
	int responseCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	if(isErrorCode(responseCode)){
		emit error();
	}

	if(movedUrl.isEmpty()){
		QByteArray res = reply->readAll();
		emit downloaded(res);
		return;
	}else if(!isErrorCode(responseCode)){
		m_request->setUrl(movedUrl);
		download();
	}
}

void Downloader::downloadProgressSlot(qint64 read, qint64 total){
	int r = rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	if(r == 200)
		emit downloadProgress(read, total);
}
