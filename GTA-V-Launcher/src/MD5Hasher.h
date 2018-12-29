#ifndef MD5HASHER_H
#define MD5HASHER_H

#include <QFile>
#include <QObject>
#include <QList>
#include <QString>
#include <QFutureWatcher>
#include <QMutex>

class MD5Hasher : public QObject
{
		Q_OBJECT

	public:
		MD5Hasher();
		~MD5Hasher();
		static QMutex s_finishedMutex;
		bool isStopped() const;
		void stop();
		inline qint64 getSize() const { return _size; }
		void addFile(QString const& file);
		void addFiles(QList<QString> const& files, QString const& base);
		void process() const;
		bool isRunning() const;
	private:
		QFutureWatcher<QPair<QString, QString>> *_md5FutureWatcher;
		QList<QFile*> _files;
		qint64 _size = 0;
		bool _isStopped = false;
		bool _shouldDelete = false;
		QPair<QString, QString> hash(QFile* file) const;
		void init();
	Q_SIGNALS:
		void fileProcessing(QString const& file) const;
		void bytesProcessing(qint64 bytes) const;
		void finished(QList<QPair<QString, QString>> const& result) const;
	public Q_SLOTS:
		void deleteLater();
};
#endif // MD5HASHER_H
