#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <iostream>

int main(int argc, char *argv[]){
	QApplication app(argc, argv);

	QString locale = QLocale::system().name().section("_", 0, 0);

	QTranslator translator;
	translator.load(":/translates/" + locale);

	QTranslator translator2;
	std::cout << QLibraryInfo::location(QLibraryInfo::TranslationsPath).toStdString().c_str() << std::endl;
	translator2.load("qt_fr", QLibraryInfo::location(QLibraryInfo::TranslationsPath));


	qApp->installTranslator(&translator2);
	qApp->installTranslator(&translator);

	app.setApplicationVersion(APP_VERSION);

	MainWindow w;
	w.init();

	return app.exec();
}
