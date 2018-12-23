#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include "TranslatorAliases.h"
#include <QSsl>
#include <QDebug>
#include <openssl/evp.h>
#include "MD5Hasher.h"


int main(int argc, char *argv[]){

	QApplication app(argc, argv);

	qDebug() << "Support SSL:  " << QSslSocket::supportsSsl()
			<< "\nLib Version String: " << QSslSocket::sslLibraryVersionString()
			<< "\nLib Build Version String: " << QSslSocket::sslLibraryBuildVersionString();


	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();

	TranslatorAliases::loadSavedLanguage();

	app.setApplicationVersion(APP_VERSION);

	MainWindow w;
	w.init();

	return app.exec();
}
