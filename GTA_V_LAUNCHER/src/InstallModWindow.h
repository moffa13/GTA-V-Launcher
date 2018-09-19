#ifndef INSTALLMODWINDOW_H
#define INSTALLMODWINDOW_H

#include <QDialog>
#include <QDir>
#include <QHideEvent>
#include "QCheckableFileSystemModel.h"
#include "QFileSystemModelDirectorySortProxy.h"

namespace Ui {
class InstallModWindow;
}

typedef struct{
		QStringList _detectedAsi;
		QStringList _detectedDll;
} modsStruct;

enum ModType{
	NONE,
	ASI,
	DLL,
	BOTH
};

class InstallModWindow : public QDialog
{
		Q_OBJECT

	public:
		explicit InstallModWindow(QString const& installDir, QString const& modsDir, QString const& scriptsDir, QWidget *parent = nullptr);
		~InstallModWindow() override;
	protected:
		void hideEvent(QHideEvent *event) override;
	private slots:
		void validateEdit(const QString &text);
		void addMod();
	Q_SIGNALS:
		void modAdded(QString const& name) const;
	private:
		Ui::InstallModWindow *ui;
		QDir const _modsDir;
		QDir const _scriptsDir;
		QDir _installDir;
		QDir _currentDir;
		QCheckableFileSystemModel *_model = nullptr;
		QFileSystemModelDirectorySortProxy *_sortModel = nullptr;
		ModType _type;
		void clearInstallDirectory(bool mk = true);
		void copyAndExtractZip(const QString &zip) const;
		modsStruct detectModFiles() const;
		QSet<QString> detectNeededFiles(QDir _installDir, modsStruct detectedMods, bool takeAllConfigFiles);
		static void copyDir(QDir const& from, QDir const& to);
		void initFileSystemModel();
};

#endif // INSTALLMODWINDOW_H
