#include "MD5Hasher.h"


#include <QtConcurrent>
#include <QFuture>
#include <sstream>
#include <QMutexLocker>
#include <iomanip>
#include "openssl/md5.h"

#define BUFFER_SIZE 268'435'456

QMutex MD5Hasher::s_finishedMutex;

MD5Hasher::MD5Hasher(){
	init();
}

MD5Hasher::~MD5Hasher(){
	_md5FutureWatcher->deleteLater();
}

bool MD5Hasher::isFinished() const{
	QMutexLocker locker{&s_finishedMutex};
	return _isStopped;
}

void MD5Hasher::stop(){
	QMutexLocker locker{&s_finishedMutex};
	_isStopped = true;
	_md5FutureWatcher->cancel();
}

void MD5Hasher::init(){
	_md5FutureWatcher = new QFutureWatcher<QPair<QString, QString>>;
	connect(_md5FutureWatcher, &QFutureWatcher<QPair<QString, QString>>::finished, [this](){
		if(!_md5FutureWatcher->isFinished())
			return;
		_files.clear();
		_size = 0;
		emit finished(_md5FutureWatcher->future().results());
	});
}

void MD5Hasher::addFile(const QString &file){
	QFile *f  = new QFile(file);
	_size += f->size();
	_files.append(f);
}

void MD5Hasher::addFiles(const QList<QString> &files, QString const& base){
	for(QString const& file : files){
		QFile *f  = new QFile(QString{"%1/%2"}.arg(base).arg(file));
		_size += f->size();
		_files.append(f);
	}
}

void MD5Hasher::process(){

	if(_files.isEmpty()) return;

	std::function<QPair<QString, QString>(QFile* file)> func = [this](QFile* file) -> QPair<QString, QString> {
		return hash(file);
	};


	QFuture<QPair<QString, QString>> future = QtConcurrent::mapped(_files, func);
	_md5FutureWatcher->setFuture(future);

}

QPair<QString, QString> MD5Hasher::hash(QFile* file){

	if(!isFinished()) emit fileProcessing(file->fileName());

	if(!file->open(QFile::ReadOnly)) return QPair<QString, QString>(file->fileName(), "");

	MD5_CTX ctx;
	MD5_Init(&ctx);
	char *data = new char[BUFFER_SIZE];
	memset(data, 0, BUFFER_SIZE);
	qint64 toRead = file->size();
	bool finished = false;
	while(!finished && toRead > 0){
		auto readLength = file->read(data, BUFFER_SIZE);
		toRead -= readLength;
		MD5_Update(&ctx, data, readLength);
		memset(data, 0, BUFFER_SIZE);
		finished = isFinished();
		if(!finished) emit bytesProcessing(readLength);
	}

	if(!finished){
		unsigned char* md5 = new unsigned char[16];
		MD5_Final(md5, &ctx);
		file->close();

		std::stringstream ss;
		for(int i = 0; i < 16; ++i){
			ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(md5[i]);
		}
		delete[] md5;
		delete[] data;

		return QPair<QString, QString>(file->fileName(), QString{ss.str().c_str()});
	}

	delete[] data;

	return  QPair<QString, QString>("", "");

}
