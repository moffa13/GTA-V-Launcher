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
		static const QHash<QString, QString> s_hashes;
		inline qint64 getSize() const { return _hasher.getSize(); }
		void check() const;
		inline void stop() { _hasher.stop(); }
	private:
		QString const _base;
		MD5Hasher _hasher;
	Q_SIGNALS:
		void fileProcessing(QString const& file) const;
		void bytesProcessing(qint64 bytes) const;
		void error(QString const& file) const;
		void success();
	private Q_SLOTS:
		void process(QList<QPair<QString, QString>> const& result);

};

#endif // GTAFILESCHECKER_H
