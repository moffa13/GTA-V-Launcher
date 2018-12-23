#ifndef TRANSLATORALIASES_H
#define TRANSLATORALIASES_H

#include <QString>
#include <QHash>
#include <QTranslator>
#include <memory>


class TranslatorAliases
{
	public:
		static void loadLanguage(QString const& locale, bool aliases, QString localeName = QString());
		static void loadSavedLanguage();
		static QList<QPair<QString, QString>> getAvailableLanguages();
		static QString getLoadedLanguage();
		static void loadSystemLanguage();
	private:
		static QString getQM(QString const& locale);
		static const QHash<QString, QString> _aliases;
		static const QList<QPair<QString, QString>> _trFilesToString;
		static std::unique_ptr<QTranslator> _QtTranslator;
		static std::unique_ptr<QTranslator> _translator;
		static QString _loadedLanguage;
		static void saveLoadedLanguage();
};

#endif // TRANSLATORALIASES_H
