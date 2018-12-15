#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include "TranslatorAliases.h"
#include <QSsl>
#include <QDebug>


int main(int argc, char *argv[]){

	QApplication app(argc, argv);

	qDebug() << "Support SSL:  " << QSslSocket::supportsSsl()
			<< "\nLib Version String: " << QSslSocket::sslLibraryVersionString()
			<< "\nLib Build Version String: " << QSslSocket::sslLibraryBuildVersionString();

	QString locale = TranslatorAliases::getQM(QLocale::system().name());

	QTranslator translator;
	translator.load(QString{":/translates/%1"}.arg(locale));

	QTranslator translator2;
	translator2.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));

	qApp->installTranslator(&translator2);
	qApp->installTranslator(&translator);

	app.setApplicationVersion(APP_VERSION);

	MainWindow w;
	w.init();

	return app.exec();
}
