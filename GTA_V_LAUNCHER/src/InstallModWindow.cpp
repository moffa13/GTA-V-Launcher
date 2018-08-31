#include "InstallModWindow.h"
#include "ui_InstallModWindow.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QtDebug>
#include <QProcess>
#include <QStack>

InstallModWindow::InstallModWindow(QString installDir, QString modsDir, QString scriptsDir, QWidget *parent) :
	QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
	ui(new Ui::InstallModWindow),
	_installDir{installDir},
	_modsDir{modsDir},
	_scriptsDir{scriptsDir}
{
	ui->setupUi(this);

	setWindowTitle(tr("Install a mod"));
	ui->selectFileButton->setText(tr("Select a mod file (*.asi, *.dll, *.zip)"));
	ui->validateButton->setText(tr("Confirm"));

	connect(ui->selectFileButton, &QPushButton::clicked, [this](){
		QString file = QFileDialog::getOpenFileName(
				this,
				tr("Select your mod file"),
				QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first(),
				"ASI mod files (*.asi *.dll *.zip)"
		);
		ui->modFileEdit->setText(file);
	});

	connect(ui->validateButton, SIGNAL(clicked(bool)), this, SLOT(addMod()));

	connect(ui->modFileEdit, SIGNAL(textChanged(QString)), this, SLOT(validateEdit(QString)));

	ui->modFileEdit->setMinimumWidth(300);

	setFixedSize(sizeHint());

}

void InstallModWindow::clearInstallDirectory(){
	_installDir.removeRecursively();
	_installDir.mkdir(".");
}

void InstallModWindow::addMod() const{
	for(int i = 0; i < ui->filesInZipList->count(); ++i){
		QListWidgetItem* itemP = ui->filesInZipList->item(i);
		if(itemP->checkState() == Qt::Unchecked) continue;
		QString item = itemP->text();

		QFileInfo fInfo{_currentDir.absoluteFilePath(item)};
		if(fInfo.isFile() && (item.endsWith(".asi") || _type == ASI)){
			if(item.endsWith(".asi")){
				emit modAdded(item);
			}
			QFile::copy(_currentDir.absoluteFilePath(item), _modsDir.absoluteFilePath(item));
		}else if(fInfo.isFile() && (item.endsWith(".dll") || _type == DLL)){
			QFile::copy(_currentDir.absoluteFilePath(item), _scriptsDir.absoluteFilePath(item));
		}else if(fInfo.isDir()){ // Config Files which we don't know where to copy
			copyDir(QDir{_currentDir.absoluteFilePath(item)}, _scriptsDir.absoluteFilePath(item));
		}else{

		}
	}
}

void InstallModWindow::addToList(QMap<QString, bool> elements) const{
	for(auto e : elements.toStdMap()){
		QListWidgetItem* item = new QListWidgetItem(e.first, ui->filesInZipList);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(e.second ? Qt::Checked : Qt::Unchecked);
		ui->filesInZipList->addItem(item);
	}
}

void InstallModWindow::copyDir(const QDir &from, const QDir &to){
	to.mkpath(".");
	QFileInfoList files = from.entryInfoList(QStringList() <<"*", QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
	QStack<QFileInfo> stack;
	for(auto const& file : files){
		stack.push(file);
	}
	while(!stack.isEmpty()){
		auto file = stack.pop();
		if(!file.isDir()){
			QString absolute = file.absolutePath();
			QString relative = absolute.right(absolute.length() - from.absolutePath().length());
			to.mkpath(relative);
			QFile::copy(file.absoluteFilePath(), to.absolutePath() + relative + "/" + file.fileName());
		}
	}
}

/**
 * Copies the zip to the install dir & extracts it
 * @brief InstallModWindow::copyAndExtractZip
 * @param zip
 */
void InstallModWindow::copyAndExtractZip(QString const& zip) const{
	QFile::copy(":/unzip.exe", _installDir.absolutePath() + "/unzip.exe");
	QFile::copy(zip, _installDir.absolutePath() + "/mod.zip");
	// Extract all the files in a single directory
	QProcess process;
	process.setWorkingDirectory(_installDir.absolutePath());
	process.start(_installDir.absolutePath() + "/unzip.exe",
			QStringList() << "-o" << "mod.zip"
	);
	process.waitForFinished();
}

/**
 * @brief InstallModWindow::detectModFiles
 * @return
 */
modsStruct InstallModWindow::detectModFiles() const{
	// Only list files
	auto files = _installDir.entryList(QStringList() << "*.*", QDir::Files | QDir::NoDotAndDotDot);
	// List asi files in the first loop for late uses
	QStringList detectedAsi;
	QStringList detectedDll;
	for(QString const& file : files){
		if(file.endsWith(".asi")){
			detectedAsi << file.left(file.lastIndexOf(".asi"));
		}else if(file.endsWith(".dll")){
			detectedDll << file.left(file.lastIndexOf(".dll"));
		}
	}

	return {
		detectedAsi,
		detectedDll
	};
}

QMap<QString, bool> InstallModWindow::detectNeededFiles(QDir _installDir, modsStruct detectedMods, bool takeAllConfigFiles){
	bool hasAsi = !detectedMods._detectedAsi.empty();
	bool hasDll = !detectedMods._detectedDll.empty();
	if(hasAsi && hasDll){
		_type = BOTH;
	}else if(hasAsi && !hasDll){
		_type = ASI;
	}else if(hasDll && !hasAsi){
		_type = DLL;
	}else{
		_type = NONE;
	}

	QMap<QString, bool> files;
	for(QString const& mod : detectedMods._detectedAsi){
		files[mod + ".asi"] = true;
	}

	for(QString const& mod : detectedMods._detectedDll){
		files[mod + ".dll"] = true;
	}


	QFileInfoList filesInfos = _installDir.entryInfoList(QStringList() << "*", QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	for(QFileInfo const& fileInfos : filesInfos){
		QString const file = fileInfos.fileName();
		bool isFile = fileInfos.isFile();
		// No need of these files
		if(isFile && file == "unzip.exe" || file == "mod.zip") continue;

		if(takeAllConfigFiles)
			files[file] = false;

		// File is either a raw mod or a config file
		if(isFile && takeAllConfigFiles && (file.endsWith(".asi") || file.endsWith(".dll") || file.endsWith(".xml") || file.endsWith(".ini"))){
			files[file] = true;
		} else if(!isFile && (detectedMods._detectedDll.contains(file))){
			files[file] = true;
		}else if(takeAllConfigFiles){
			// If the file also starts with the name of a mod file, take it
			for(auto const& mod : detectedMods._detectedAsi + detectedMods._detectedDll){
				if(file.startsWith(mod)){
					files[file] = true;
				}
			}
		}
	}
	return files;
}

void InstallModWindow::validateEdit(QString const& text){
	QFile f{text};
	if(!f.exists()) return;

	ui->filesInZipList->clear();

	QMap<QString, bool> neededFiles;

	if(text.endsWith(".zip")){
		_currentDir = _installDir;
		// Remove the install dir & re-create it
		clearInstallDirectory();
		// Copy unzip & the mod
		copyAndExtractZip(text);

		modsStruct detectedMods{detectModFiles()};

		neededFiles = detectNeededFiles(_installDir, detectedMods, true);

	}else if(text.endsWith(".asi") || text.endsWith(".dll")){
		QFileInfo f{text};
		QDir parent{f.absoluteDir()};
		_currentDir = parent;

		QString const modNameWithoutExtension{f.fileName().left(f.fileName().lastIndexOf("."))};

		modsStruct mods;
		if(text.endsWith(".asi")){
			mods._detectedAsi << modNameWithoutExtension;
		}else{
			mods._detectedDll << modNameWithoutExtension;
		}


		neededFiles = detectNeededFiles(parent, mods, false);
	}

	addToList(neededFiles);
}

void InstallModWindow::hideEvent(QHideEvent *e){
	e->accept();
	deleteLater();
}

InstallModWindow::~InstallModWindow(){
	delete ui;
}
