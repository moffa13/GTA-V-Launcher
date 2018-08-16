#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

int main(int argc, char *argv[]){
	QApplication app(argc, argv);

	QString locale = QLocale::system().name().section("_", 0, 0);
	QTranslator translator;
	translator.load(":/translates/"+locale);
	app.installTranslator(&translator);

	QTranslator translator2;
	translator2.load(QString("qtbase_")+locale+".qm", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	app.installTranslator(&translator2);

	app.setApplicationVersion(APP_VERSION);

	MainWindow w;
	w.init();

	return app.exec();
}
