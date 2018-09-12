#include "DirsInfos.h"
#include <QFileInfo>
#include <QFileInfoList>
#include <QStringList>
#include <QStack>
#include <QDir>
#include <QMapIterator>

DirsInfos::DirsInfos(const QString &dir){
	QFileInfo info{dir};
	if(!info.exists() || !info.isDir()) throw std::runtime_error{"Not a directory or do not exist"};
	_baseDir = QDir{dir};
	iterateThroughDir();
}

void DirsInfos::iterateThroughDir(){
	QDir::Filters entryFlags{QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs | QDir::Hidden};
	QFileInfoList objects = _baseDir.entryInfoList(entryFlags);
	QStack<QFileInfo> stack;

	for(QFileInfo const& fInfo : objects){
		stack.push(fInfo);
	}

	while(!stack.isEmpty()){
		QFileInfo info{stack.pop()};
		if(!info.isDir()){
			add(info.absoluteFilePath(), false);
		}else{
			bool empty = true;
			QFileInfoList newFiles{QDir{info.absoluteFilePath()}.entryInfoList(entryFlags)};
			for(QFileInfo const& file : newFiles){
				empty = false;
				stack.push(file);
			}
			if(empty){
				add(info.absoluteFilePath(), true);
			}
		}
	}
}

QStringList DirsInfos::relativeFromAbsolute(QString const& absolute){
	QString split = absolute.right(absolute.length() - _baseDir.absolutePath().length() - 1);
	return split.split("/");
}

void DirsInfos::add(const QString &name, bool directory){
	QStringList hierarchy = relativeFromAbsolute(name);

	// File at root
	if(hierarchy.size() == 1 && !directory){
		_dirs._files.append(hierarchy[0]);
		return;
	}

	int i = 0;
	dirInfos* currentDir = &_dirs;
	while(i < hierarchy.size()){
		QString const toFind = hierarchy.at(i);
		if(i == hierarchy.size() - 1){ // We reach the end
			if(!directory && !currentDir->_files.contains(toFind)){
				currentDir->_files.append(toFind);
			}else if(!currentDir->_dirs.contains(toFind)){
				currentDir->_dirs.insert(toFind, new dirInfos{toFind});
			}
		}else{ // We must create the hierarchy if it still does not exist
			if(!currentDir->_dirs.contains(toFind)){
				currentDir->_dirs.insert(toFind, new dirInfos{toFind});
			}
			currentDir = currentDir->_dirs[toFind];
		}
		++i;
	}

}

QString DirsInfos::toString(){
	return toString(&_dirs);
}

QString DirsInfos::toString(dirInfos *infos){
	QString final;
	int i = 0;
	final += infos->_name + "\r\n";
	for(QString const& file : infos->_files){
		final += file + "\r\n";
	}
	QMapIterator<QString, dirInfos*> it{infos->_dirs};
	while(it.hasNext()){
		it.next();
		final += toString(it.value());
	}
	++i;
	return final;
}
