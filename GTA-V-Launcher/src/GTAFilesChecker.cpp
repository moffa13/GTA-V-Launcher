#include "GTAFilesChecker.h"

const QHash<QString, QString> GTAFilesChecker::s_hashes = {
	{"bink2w64.dll", "3722f03c488093cb2631b5412d4f12d9"},
	{"common.rpf", "bbe724acef5f9fcfd85a38c4e35e2f00"},
	{"d3dcompiler_46.dll", "7ea872c2f9803cfb4223098b85e70cc0"},
	{"d3dcsx_46.dll", "8355e491fa90ca00045be22bb556b213"},
	{"GFSDK_ShadowLib.win64.dll", "f2c348c5aaff0c420f4dce3abc1bbad6"},
	{"GFSDK_TXAA.win64.dll", "167385d13443035ff68643b2c0c59a4d"},
	{"GFSDK_TXAA_AlphaResolve.win64.dll", "ea04393624856f44854cace25b50ce3c"},
	{"GPUPerfAPIDX11-x64.dll", "121044fe4ae47114dfccd15e399df399"},
	{"NvPmApi.Core.win64.dll", "2041025c15e6ff893dcbb5ed63fdb2f8"},
	{"x64a.rpf", "abc"},
	{"x64b.rpf", "70af24cd4fe2c8ee58edb902f018a558"}
};

GTAFilesChecker::GTAFilesChecker(QString base) : _base{std::move(base)}{

	_hasher = new MD5Hasher;

	connect(_hasher, &MD5Hasher::finished, this, &GTAFilesChecker::process);

	// SIGNALS REDIRECT
	connect(_hasher, &MD5Hasher::fileProcessing, this, &GTAFilesChecker::fileProcessing);
	connect(_hasher, &MD5Hasher::bytesProcessing, this, &GTAFilesChecker::bytesProcessing);

	_hasher->addFiles(s_hashes.keys(), _base);
}

GTAFilesChecker::~GTAFilesChecker(){
	_hasher->deleteLater();
}

void GTAFilesChecker::check() const{
	_hasher->process();
}

void GTAFilesChecker::stop(){
	disconnect(_hasher, 0, 0, 0);
	_hasher->stop();
}

QStringList GTAFilesChecker::getErrors() const{
	return _md5Errors;
}

bool GTAFilesChecker::deleteCorrupted() const{
	if(_md5Errors.isEmpty()) return true;
	bool deleteOk = true;
	for(auto const& file : _md5Errors){
		deleteOk &= QFile(QString("%1/%2").arg(_base).arg(file)).remove();
	}
	return deleteOk;
}

void GTAFilesChecker::process(const QList<QPair<QString, QString>> &result){
	if(_hasher->isFinished()) return; // If it has been canceled, do not process
	_md5Errors.clear();

	bool isError = false;

	for(QList<QPair<QString, QString>>::const_iterator it = result.constBegin(); it != result.constEnd(); ++it){
		QString const relFile = it->first.mid(_base.length() + 1);
		if(s_hashes[relFile] != it->second){
			_md5Errors << relFile;
			isError = true;
			qDebug() << relFile << "error md5 " << it->second << " should be " << s_hashes[relFile];
		}
	}

	if(isError)
		emit error();
	else
		emit success();
}
