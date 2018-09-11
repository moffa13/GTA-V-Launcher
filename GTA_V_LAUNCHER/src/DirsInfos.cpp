#include "DirsInfos.h"
#include <QFileInfo>

DirsInfos::DirsInfos(const QString &dir){
	QFileInfo info{dir};
	if(!info.exists() || !info.isDir()) throw std::exception{"Not a directory or do not exist"};
	iterateThroughDir();
}

void DirsInfos::iterateThroughDir(){

}

void DirsInfos::add(const QString &name, bool directory){

}
