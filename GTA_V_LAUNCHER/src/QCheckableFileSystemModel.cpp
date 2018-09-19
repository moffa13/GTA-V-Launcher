#include "QCheckableFileSystemModel.h"
#include <QAbstractItemModel>
#include <QStack>
#include <QEventLoop>
#include <QtDebug>

QCheckableFileSystemModel::QCheckableFileSystemModel(QObject* parent) : QFileSystemModel (parent)
{}

QVariant QCheckableFileSystemModel::data(const QModelIndex &index, int role) const{
	// Item is disabled or it's a column we don't need
	if((index.flags() & Qt::ItemIsEnabled) != Qt::ItemIsEnabled || index.column() != 0){
		return QVariant{};
	}

	if(role == Qt::CheckStateRole && index.column() == 0){ // We need to return if item is checked or not
		int checked = _indexesCheckedStates.contains(index) ? _indexesCheckedStates[index] : Qt::Unchecked;
		return checked;
	}
	return QFileSystemModel::data(index, role); // Default behavior
}

bool QCheckableFileSystemModel::setData(const QModelIndex &index, const QVariant &value, int role){

	if(role == Qt::CheckStateRole && index.column() == 0 && index.isValid()){

		_indexesCheckedStates[index] = value.toInt();

		// Parent to parent order check
		QModelIndex current = index;

		while(current.parent().isValid()){
			bool parentChecked = true;
			bool atLeastOneIsChecked = false;
			for(int i = 0; i < rowCount(current.parent()); ++i){
				Qt::CheckState chkst = static_cast<Qt::CheckState>(data(this->index(i, 0, current.parent()), Qt::CheckStateRole).toInt());
				if(chkst == Qt::PartiallyChecked){
					parentChecked = false;
					atLeastOneIsChecked = true;
					break;
				}
				bool st = chkst == Qt::Checked ? true : false;
				parentChecked &= st;
				atLeastOneIsChecked |= st;
			}
			if(parentChecked){
				_indexesCheckedStates[current.parent()] = Qt::Checked;
			}else if(atLeastOneIsChecked){
				_indexesCheckedStates[current.parent()] = Qt::PartiallyChecked;
			}else{
				_indexesCheckedStates[current.parent()] = Qt::Unchecked;
			}
			emit dataChanged(current.parent(), current.parent());
			current = current.parent();
		}

		//Parent to child

		// If it is not a dir, no need to check for children
		if(isDir(index)){
			Qt::CheckState state = static_cast<Qt::CheckState>(data(index, Qt::CheckStateRole).toInt());
			QStack<QModelIndex> toProcess;
			toProcess.push(index);
			while(!toProcess.empty()){
				QModelIndex toTreat = toProcess.pop();

				discover(toTreat);

				if(isDir(toTreat) && hasChildren(toTreat)){
					for(int i = 0; i < rowCount(toTreat); ++i){
						QModelIndex child = this->index(i, 0, toTreat);
						if(isDir(child)){
							toProcess.push(child);
						}
						_indexesCheckedStates[child] = state;
						emit dataChanged(child, child);
					}
				}
			}
		}


		emit dataChanged(index, index);
		return true;
	}
	return QFileSystemModel::setData(index, value, role);
}



void QCheckableFileSystemModel::discover(QModelIndex const& index){
	while(canFetchMore(index)){
		QEventLoop loop;
		connect(this, &QFileSystemModel::directoryLoaded, &loop, &QEventLoop::quit);
		fetchMore(index);
		loop.exec();
	}
}

Qt::ItemFlags QCheckableFileSystemModel::flags(const QModelIndex &index) const{
	return QFileSystemModel::flags(index) | Qt::ItemIsUserCheckable;
}
