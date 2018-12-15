#ifndef TRANSLATORALIASES_H
#define TRANSLATORALIASES_H

#include <QString>
#include <QHash>


class TranslatorAliases
{
	public:
		static QString getQM(QString const& locale);
	private:
		static const QHash<QString, QString> _aliases;
};

#endif // TRANSLATORALIASES_H
