#include "TranslatorAliases.h"

QHash<QString, QString> const TranslatorAliases::_aliases = {
	{"fr_BE", "fr"},
	{"fr_CA", "fr"},
	{"fr_CH", "fr"},
	{"fr_FR", "fr"},
	{"fr_LU", "fr"}
};

QString TranslatorAliases::getQM(const QString &locale){
	if(!_aliases.contains(locale)) return locale;
	return _aliases[locale];
}
