#ifndef QFILESYSTEMMODELDIRECTORYSORTPROXY_H
#define QFILESYSTEMMODELDIRECTORYSORTPROXY_H

#include <QSortFilterProxyModel>


class QFileSystemModelDirectorySortProxy : public QSortFilterProxyModel
{
	public:
		QFileSystemModelDirectorySortProxy(QObject *parent = nullptr);
		void setAcceptedDirs(QStringList const& list);
		bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
	private:
		QStringList _acceptedDirs;
};

#endif // QFILESYSTEMMODELDIRECTORYSORTPROXY_H
