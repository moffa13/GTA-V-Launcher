#ifndef MD5HASHER_H
#define MD5HASHER_H

#include <QFile>
#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QFutureWatcher>
#include <QMutex>

class MD5Hasher : public QObject
{
		Q_OBJECT

	public:
		MD5Hasher();
		~MD5Hasher();
		static QMutex s_finishedMutex;
		bool isFinished() const;
		void stop();
		inline qint64 getSize() const { return _size; }
		void addFile(QString const& file);
		void addFiles(QList<QString> const& files, QString const& base);
		void process();
	private:
		QList<QFile*> _files;
		qint64 _size = 0;
		bool _isStopped = false;
		QFutureWatcher<QPair<QString, QString>> *_md5FutureWatcher;
		QPair<QString, QString> hash(QFile* file);
		void init();
	Q_SIGNALS:
		void fileProcessing(QString const& file);
		void bytesProcessing(qint64 bytes);
		void finished(QList<QPair<QString, QString>> const& result);
};
#endif // MD5HASHER_H
