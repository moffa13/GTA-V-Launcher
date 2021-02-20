#include "TranslatorAliases.h"

#include <QApplication>
#include <QLibraryInfo>
#include <QDir>
#include <QtDebug>
#include "Utilities.h"

QHash<QString, QString> const TranslatorAliases::_aliases = {
	{"fr_BE", "fr"},
	{"fr_CA", "fr"},
	{"fr_CH", "fr"},
	{"fr_FR", "fr"},
	{"fr_LU", "fr"}
};

QList<QPair<QString, QString>> const TranslatorAliases::_trFilesToString = {
	{"English", "en"},
	{"Français", "fr"},
	{"Português (Brasil)", "pt_BR"},
	{"русский", "ru"}
};

std::unique_ptr<QTranslator> TranslatorAliases::_translator;
std::unique_ptr<QTranslator> TranslatorAliases::_QtTranslator;

QString TranslatorAliases::_loadedLanguage = "en";

QString TranslatorAliases::getQM(const QString &locale){
	if(!_aliases.contains(locale)) return locale;
	return _aliases[locale];
}

QList<QPair<QString, QString>> TranslatorAliases::getAvailableLanguages(){
	return _trFilesToString;
}

void TranslatorAliases::loadLanguage(const QString &locale, bool aliases, QString localeName){
	if(localeName.isEmpty())
		_loadedLanguage = locale;
	else
		_loadedLanguage = localeName;
	if(!_QtTranslator) _QtTranslator.reset(new QTranslator);
	if(!_translator) _translator.reset(new QTranslator);
	qApp->removeTranslator(_QtTranslator.get());
	qApp->removeTranslator(_translator.get());
	_translator->load(QString{":/translates/%1"}.arg(aliases ? TranslatorAliases::getQM(locale) : locale));
	_QtTranslator->load("qt_" + locale.section("_", 0, 0), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	qApp->installTranslator(_QtTranslator.get());
	qApp->installTranslator(_translator.get());
	saveLoadedLanguage();
}

void TranslatorAliases::loadSystemLanguage(){
	loadLanguage(QLocale::system().name(), true, "default");
}

void TranslatorAliases::saveLoadedLanguage(){
	Utilities::setToConfig("General", QMap<QString, QVariant>{{"Language", _loadedLanguage}});
}

QString TranslatorAliases::getLoadedLanguage(){
	return _loadedLanguage;
}

void TranslatorAliases::loadSavedLanguage(){
	QString language = Utilities::loadFromConfig("General", "Language", QString()).toString();
	if(language.isEmpty() || language == "default"){
		loadSystemLanguage();
	}else{
		loadLanguage(language, false);
	}
}
