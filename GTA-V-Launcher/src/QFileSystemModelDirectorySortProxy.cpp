#include "QFileSystemModelDirectorySortProxy.h"
#include "QCheckableFileSystemModel.h"
#include <QFileInfo>
#include <QtDebug>

QFileSystemModelDirectorySortProxy::QFileSystemModelDirectorySortProxy(QObject *parent) : QSortFilterProxyModel (parent)
{}

void QFileSystemModelDirectorySortProxy::setAcceptedDirs(const QStringList &list)
{
	_acceptedDirs = list;
}

bool QFileSystemModelDirectorySortProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{

	if(_acceptedDirs.isEmpty()) return true;

	QCheckableFileSystemModel *sourceModel = static_cast<QCheckableFileSystemModel*>(this->sourceModel());
	QString const path = sourceModel->filePath(source_parent);
	QString const root = sourceModel->rootPath();

	if(path.length() != root.length()) return true;

	QString const childPath = sourceModel->filePath(sourceModel->index(source_row, 0, source_parent));

	QFileInfo d{childPath};
	return _acceptedDirs.contains(d.fileName());

}
