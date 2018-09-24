#ifndef CHOOSEMODSWINDOW_H
#define CHOOSEMODSWINDOW_H

#include "InstallModWindow.h"
#include "SelfDeleteDialog.h"
#include <QLabel>
#include <QListView>
#include <QPushButton>
#include <QStringList>
#include <QString>
#include <QStringListModel>
#include <QList>
#include <QPair>
#include <QSettings>
#include "Version.h"

namespace Ui{
class ChooseModsWindow;
}


class ChooseModsWindow : public SelfDeleteDialog{

	Q_OBJECT

	public:
		ChooseModsWindow(QWidget *parent = nullptr);
		~ChooseModsWindow();
		void init();
		static void disableAllMods();
		static void enableAllMods();
		static void enableOldConfig();
	private:
		QStringListModel *m_modele1 = nullptr;
		QStringListModel *m_modele2 = nullptr;
		InstallModWindow *m_installModWindow = nullptr;
		QList<QPair<QString, Version>> m_enabledModsAndVersions;
		QList<QPair<QString, Version>> m_disabledModsAndVersions;
		static const QString m_gtaDirectoryStr;
		static const QString m_disabledModsDirectoryStr;
		QModelIndex m_lastIndex;
		void connectAll();
		void getFromFiles();
		void setEnabledModsToList(const QList<QPair<QString, Version>> &enabledMods);
		void setDisabledModsToList(const QList<QPair<QString, Version>> &enabledMods);
		static QStringList getEnabledModsFromFiles();
		static QStringList getDisabledModsFromFiles();
		static void noConflicts(QStringList &enableMods, QStringList &disabledMods);
		static void noConflicts(QStringList &enableMods, QStringList &disabledMods, QSet<QString> const& conflicts);
		void enableButton(QString btn);
		void enableDisableMod(QStringListModel *model);
		static void saveMods(QList<QPair<QString, Version>> const& enabledModsAndVersions, QList<QPair<QString, Version>> const& disabledModsAndVersions);
		static void saveMods(QStringList const& disable, QStringList const& enable = QStringList());
		QStringList getEnabledModsFromList();
		QStringList getDisabledModsFromList();
		static QStringList checkModsExists(QStringList &list);
		QList<QPair<QString, Version>> addVersionToElements(QString const& baseAsi, QString const& baseDll, QStringList const& list) const;
		QList<QPair<QString, Version> > addVersionToElements(const QString &base, const QStringList &list) const;
		static QStringList toQStringList(const QList<QPair<QString, Version> > &list, bool addVersion = false);
		Ui::ChooseModsWindow* ui;
		static QPair<QStringList, QStringList> getModsConfig();
		void setEnableDisableAllButtons();
		static QString basePathFromModType(const QString &mod);
	public slots:
		void setModsSlot();
		void loadConfigSlot();
		void setConfigSlot();
		void setButtonUpDownSlot(QModelIndex index);
		void deleteModSlot();
};

#endif // CHOOSEMODSWINDOW_H
