#include <QApplication>
#include "MainWindow.h"
#include <QTranslator>

int main(int argc, char *argv[]){
	QApplication app(argc, argv);

	QString locale = QLocale::system().name().section("_", 0, 0);
	QTranslator translator;
	translator.load(":/translates/"+locale);
	app.installTranslator(&translator);

	MainWindow w;
	w.init();

	return app.exec();
}
