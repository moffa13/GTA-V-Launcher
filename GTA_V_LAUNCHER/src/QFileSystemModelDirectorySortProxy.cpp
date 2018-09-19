#include "QFileSystemModelDirectorySortProxy.h"
#include "QCheckableFileSystemModel.h"
#include <QFileInfo>

QFileSystemModelDirectorySortProxy::QFileSystemModelDirectorySortProxy(QObject *parent) : QSortFilterProxyModel (parent)
{}

void QFileSystemModelDirectorySortProxy::setAcceptedDirs(const QStringList &list)
{
	_acceptedDirs = list;
}

bool QFileSystemModelDirectorySortProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{

	return true;

}
