#include "InstallModWindow.h"
#include "ui_InstallModWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QtDebug>
#include <QProcess>
#include <QStack>
#include <QFileSystemModel>

InstallModWindow::InstallModWindow(QString installDir, QString modsDir, QString scriptsDir, QWidget *parent) :
	QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
	ui(new Ui::InstallModWindow),
	_installDir{installDir},
	_modsDir{modsDir},
	_scriptsDir{scriptsDir}
{
	ui->setupUi(this);

	setWindowTitle(tr("Install a mod"));
	ui->selectFileButton->setText(tr("Select a mod file (*.asi, *.dll, *.zip, *.rar)"));
	ui->validateButton->setText(tr("Confirm"));

	connect(ui->selectFileButton, &QPushButton::clicked, [this](){
		QString file = QFileDialog::getOpenFileName(
				this,
				tr("Select your mod file"),
				QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first(),
				"ASI mod files (*.asi *.dll *.zip *.rar)"
		);
		ui->modFileEdit->setText(file);
	});

	QDir{_scriptsDir}.mkdir(".");

	connect(ui->validateButton, SIGNAL(clicked(bool)), this, SLOT(addMod()));

	connect(ui->modFileEdit, SIGNAL(textChanged(QString)), this, SLOT(validateEdit(QString)));

	ui->validateButton->setVisible(false);
	ui->filesInZipList->setVisible(false);
	ui->modFileEdit->setMinimumWidth(300);
	ui->filesInZipList->setMaximumHeight(600);

	setFixedSize(sizeHint());

}

void InstallModWindow::clearInstallDirectory(bool mk){
	_installDir.removeRecursively();
	if(mk) _installDir.mkdir(".");
}

void InstallModWindow::addMod(){
//TODO	for(int i = 0; i < ui->filesInZipList->count(); ++i){
//		QListWidgetItem* itemP = ui->filesInZipList->item(i);
//		if(itemP->checkState() == Qt::Unchecked) continue;
//		QString item = itemP->text();

//		QFileInfo fInfo{_currentDir.absoluteFilePath(item)};
//		if(fInfo.isFile() && (item.endsWith(".asi") || _type == ASI)){
//			QFile::copy(_currentDir.absoluteFilePath(item), _modsDir.absoluteFilePath(item));
//			if(item.endsWith(".asi")){
//				emit modAdded(item);
//			}
//		}else if(fInfo.isFile() && (item.endsWith(".dll") || _type == DLL)){
//			QFile::copy(_currentDir.absoluteFilePath(item), _scriptsDir.absoluteFilePath(item));
//			if(item.endsWith(".dll")){
//				emit modAdded(item);
//			}
//		}else if(fInfo.isDir()){ // Config Files which we don't know where to copy
//			copyDir(QDir{_currentDir.absoluteFilePath(item)}, _scriptsDir.absoluteFilePath(item));
//		}else{

//		}
//	}
	QMessageBox::information(this, tr("Success"), tr("Your mod has been installed with success"), QMessageBox::Ok);
	hide();
}

void InstallModWindow::addToList(QMap<QString, bool> elements) const{
	for(auto e : elements.toStdMap()){
//	TODO	QListWidgetItem* item = new QListWidgetItem(e.first, ui->filesInZipList);
//		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
//		item->setCheckState(e.second ? Qt::Checked : Qt::Unchecked);
//		ui->filesInZipList->addItem(item);
	}
}

void InstallModWindow::copyDir(const QDir &from, const QDir &to){
	to.mkpath(".");
	QFileInfoList files = from.entryInfoList(QStringList() << "*", QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
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
	QProcess process;
	process.setWorkingDirectory(_installDir.absolutePath());
	if(zip.endsWith(".rar")){
		QFile::copy(":/unrar.exe", _installDir.absolutePath() + "/unrar.exe");
		QFile::copy(zip, _installDir.absolutePath() + "/mod.rar");
		process.start(_installDir.absolutePath() + "/unrar.exe",
				QStringList() << "x" << "-o+" << "mod.rar"
		);
	}else{
		QFile::copy(":/unzip.exe", _installDir.absolutePath() + "/unzip.exe");
		QFile::copy(zip, _installDir.absolutePath() + "/mod.zip");
		process.start(_installDir.absolutePath() + "/unzip.exe",
				QStringList() << "-o" << "mod.zip"
		);
	}
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
		if(isFile && (file == "unzip.exe" || file == "unrar.exe" || file == "mod.zip" || file == "mod.rar")) continue;

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

	//TODOui->filesInZipList->model()->set

	QMap<QString, bool> neededFiles;

	if(text.endsWith(".zip") || text.endsWith(".rar")){
		_currentDir = _installDir;
		// Remove the install dir & re-create it
		clearInstallDirectory();
		// Copy unzip & the mod
		copyAndExtractZip(text);

		modsStruct detectedMods{detectModFiles()};


		QFileSystemModel *model = new QFileSystemModel;
		model->setRootPath(_installDir.absolutePath());
		model->setReadOnly(true);
		ui->filesInZipList->setModel(model);
		ui->filesInZipList->setRootIndex(model->index(_installDir.absolutePath()));





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
	ui->filesInZipList->setVisible(true);
	ui->validateButton->setVisible(true);
	setFixedSize(sizeHint());
}

void InstallModWindow::hideEvent(QHideEvent *e){
	e->accept();
	clearInstallDirectory(false);
	deleteLater();
}

InstallModWindow::~InstallModWindow(){
	delete ui;
}
