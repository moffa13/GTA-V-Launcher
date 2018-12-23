#include "SelfDeleteDialog.h"
#include "TranslatorAliases.h"
#include <QLocale>

SelfDeleteDialog::SelfDeleteDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f){}

void SelfDeleteDialog::hideEvent(QHideEvent *e){
	e->accept();
	emit hidden();
	deleteLater();
}

void SelfDeleteDialog::changeEvent(QEvent *event){
	if(event->type() == QEvent::LanguageChange){
		retranslateUi();
	}else if(event->type() == QEvent::LocaleChange && TranslatorAliases::getLoadedLanguage() == "default"){
		TranslatorAliases::loadSystemLanguage();
	}
}
