#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <memory>
#include <QSettings>
#include <QVariant>
#include <Version.h>
#include "Windows.h"

class Utilities
{
	public:
		Utilities();
		static Version getFileVersion(const QString &filename);
		static QString checkProcessRunning(const QString &name);
		static std::unique_ptr<QSettings> getSettings();
		static QMap<QString, QVariant> loadFromConfig(QString const &key);
		static QVariant loadFromConfig(QString const &key, QString const &childKey, QVariant defaultKey = QVariant{});
		static void setToConfig(QString const &key, const QStringList &data);
		static void setToConfig(QString const &key, const QMap<QString, QVariant> &data);
		static QStringList removeValues(QMap<QString, QVariant> const &map);
		static bool launcherCracked();
		static bool copy(const QString &from, const QString &to);
		static void clearConfig(const QString &key);
		static bool setProcessPriority(const QString &process, DWORD dwPriorityClass);
};
#endif // UTILITIES_H
