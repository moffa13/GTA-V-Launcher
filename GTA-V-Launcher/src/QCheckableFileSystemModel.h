#ifndef QCHECKABLEFILESYSTEMMODEL_H
#define QCHECKABLEFILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QMap>
#include <QPersistentModelIndex>


class QCheckableFileSystemModel : public QFileSystemModel
{
	public:
		QCheckableFileSystemModel(QObject* parent = nullptr);
		Qt::ItemFlags flags(const QModelIndex &index) const;
		bool setData(const QModelIndex &index, const QVariant &value, int role);
		QVariant data(const QModelIndex &index, int role) const override;
		void discover(QModelIndex const& index);
	private:
		QHash<QPersistentModelIndex, int> _indexesCheckedStates;
};

#endif // QCHECKABLEFILESYSTEMMODEL_H
