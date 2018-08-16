#ifndef CHOOSEMODSWINDOW_H
#define CHOOSEMODSWINDOW_H

#include "Window.h"
#include <QDialog>
#include <QLabel>
#include <QListView>
#include <QPushButton>
#include <QStringList>
#include <QString>
#include <QStringListModel>
#include <QList>
#include <QPair>
#include <QSettings>

namespace Ui{
class ChooseModsWindow;
}


class ChooseModsWindow : public QDialog{

	Q_OBJECT

	public:
		ChooseModsWindow(QWidget *parent = 0);
		~ChooseModsWindow();
		void init();
		static void disableAllMods();
		static void enableOldConfig();
	private:
		QStringListModel *m_modele1;
		QStringListModel *m_modele2;
		QList<QPair<QString, QString>> m_enabledModsAndVersions;
		QList<QPair<QString, QString>> m_disabledModsAndVersions;
		static const QString m_gtaDirectoryStr;
		static const QString m_disabledModsDirectoryStr;
		QModelIndex m_lastIndex;
		void connectAll();
		void closeEvent(QCloseEvent *event);
		void getFromFiles();
		void setEnabledModsToList(const QList<QPair<QString, QString>> &enabledMods);
		void setDisabledModsToList(const QList<QPair<QString, QString>> &enabledMods);
		static QStringList getEnabledModsFromFiles();
		static QStringList getDisabledModsFromFiles();
		static void noConflicts(QStringList &enableMods, QStringList &disabledMods);
		static void noConflicts(QStringList &enableMods, QStringList &disabledMods, QSet<QString> const& conflicts);
		void enableButton(QString btn);
		void enableDisableMod(QStringListModel *model);
		static void saveMods(QList<QPair<QString, QString>> const& enabledModsAndVersions, QList<QPair<QString, QString>> const& disabledModsAndVersions);
		static void saveMods(QStringList const& disable, QStringList const& enable = QStringList());
		QStringList getEnabledModsFromList();
		QStringList getDisabledModsFromList();
		static QStringList checkModsExists(QStringList &list);
		QList<QPair<QString, QString>> addVersionToElements(QString const& base, QStringList const& list) const;
		static QStringList toQStringList(const QList<QPair<QString, QString> > &list, bool addVersion = false);
		Ui::ChooseModsWindow* ui;
		static QPair<QStringList, QStringList> getModsConfig();
		void setEnableDisableAllButtons();
	public slots:
		void setModsSlot();
		void loadConfigSlot();
		void setConfigSlot();
		void setButtonUpDownSlot(QModelIndex index);
		void deleteModSlot();
};

#endif // CHOOSEMODSWINDOW_H
