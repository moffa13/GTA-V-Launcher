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

		QVector<int> roles{Qt::CheckStateRole};

		_indexesCheckedStates[index] = value.toInt(); // set data for the requested index
		emit dataChanged(index, index, roles); // emit the signal for this index

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
			current = current.parent();
			if(parentChecked){
				_indexesCheckedStates[current] = Qt::Checked;
			}else if(atLeastOneIsChecked){
				_indexesCheckedStates[current] = Qt::PartiallyChecked;
			}else{
				_indexesCheckedStates[current] = Qt::Unchecked;
			}

			emit dataChanged(current, current, roles);
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
				int rc = rowCount(toTreat);

				if(rc > 0){
					for(int i = 0; i < rc; ++i){
						QModelIndex child = this->index(i, 0, toTreat);
						if(isDir(child)){
							toProcess.push(child);
						}
						_indexesCheckedStates[child] = state;
					}

					emit dataChanged(this->index(0, 0, toTreat), this->index(rc - 1, 0, toTreat), roles);
				}
			}
		}

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
