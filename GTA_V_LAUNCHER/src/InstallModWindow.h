#ifndef INSTALLMODWINDOW_H
#define INSTALLMODWINDOW_H

#include <QDialog>
#include <QDir>
#include <QHideEvent>

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
		explicit InstallModWindow(QString installDir, QString modsDir, QString scriptsDir, QWidget *parent = 0);
		~InstallModWindow();
	protected:
		void hideEvent(QHideEvent *event) override;
	private slots:
		void validateEdit(const QString &text);
		void addMod() const;
	Q_SIGNALS:
		void modAdded(QString const& name) const;
	private:
		Ui::InstallModWindow *ui;
		QDir _installDir;
		QDir _modsDir;
		QDir _scriptsDir;
		QDir _currentDir;
		ModType _type;
		void clearInstallDirectory();
		void copyAndExtractZip(const QString &zip) const;
		modsStruct detectModFiles() const;
		QMap<QString, bool> detectNeededFiles(QDir _installDir, modsStruct detectedMods, bool takeAllConfigFiles);
		void addToList(QMap<QString, bool> elements) const;
		static void copyDir(QDir const& from, QDir const& to);
};

#endif // INSTALLMODWINDOW_H
