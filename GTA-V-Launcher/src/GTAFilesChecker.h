#ifndef GTAFILESCHECKER_H
#define GTAFILESCHECKER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QHash>
#include "MD5Hasher.h"

class GTAFilesChecker : public QObject
{
		Q_OBJECT

	public:
		GTAFilesChecker(QString base);
		~GTAFilesChecker();
		static const QHash<QString, QString> s_hashes;
		inline qint64 getSize() const { return _hasher->getSize(); }
		inline bool isRunning() const { return _hasher->isRunning(); }
		void check() const;
		void stop();
		QStringList getErrors() const;
		bool deleteCorrupted() const;
		bool rootRemoveAllUnofficialFiles();
		bool hasRealCorruptedFiles() const;
	private:
		QString const _base;
		QStringList _md5Errors;
		quint32 _existCount = 0;
		MD5Hasher *_hasher;
		bool deleteListRelative(const QStringList &list) const;
	Q_SIGNALS:
		void fileProcessing(QString const& file) const;
		void bytesProcessing(qint64 bytes) const;
		void error() const;
		void success() const;
	private Q_SLOTS:
		void process(QList<QPair<QString, QString>> const& result);

};

#endif // GTAFILESCHECKER_H
