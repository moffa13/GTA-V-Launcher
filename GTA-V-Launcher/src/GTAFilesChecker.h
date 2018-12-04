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
		inline qint64 getSize() const { return _hasher.getSize(); }
		static const QHash<QString, QString> s_hashes;
		void check();
		inline void stop() { _hasher.stop(); }
	private:
		QString _base;
		MD5Hasher _hasher;
	Q_SIGNALS:
		void fileProcessing(QString const& file);
		void bytesProcessing(qint64 bytes);
		void error(QString const& file);
		void success();
	private Q_SLOTS:
		void process(QList<QPair<QString, QString>> const& result);

};

#endif // GTAFILESCHECKER_H
