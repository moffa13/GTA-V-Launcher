#include "InstallModWindow.h"
#include "ui_InstallModWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QtDebug>
#include <QProcess>
#include <QStack>
#include "QCheckableFileSystemModel.h"
#include <QTimer>

InstallModWindow::InstallModWindow(QString const& installDir, QString const& modsDir, QString const& scriptsDir, QWidget *parent) :
	QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
	ui(new Ui::InstallModWindow),
	_modsDir{modsDir},
	_scriptsDir{scriptsDir},
	_installDir{installDir}
{
	ui->setupUi(this);

	setWindowTitle(tr("Install a mod"));
	ui->selectFileButton->setText(tr("Select a mod file (*.asi, *.dll, *.zip, *.rar)"));
	ui->validateButton->setText(tr("Confirm"));

	connect(ui->selectFileButton, &QPushButton::clicked, [this](){
		ui->modFileEdit->clear();
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

void InstallModWindow::initFileSystemModel(){

	// QTBUG-52470 can't delete folder because of QFileSystemWatcher so I have to delete the model then renew it
	delete _model;
	_model = nullptr;
	ui->filesInZipList->setModel(nullptr);


	_model = new QCheckableFileSystemModel(this);
	_model->setReadOnly(true);

	_model->setNameFilters(QStringList() << "*.asi" << "*.dll" << "*.ini" << "*.xml");
	_model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	_model->setNameFilterDisables(false);

	_sortModel = new QFileSystemModelDirectorySortProxy(this);
	_sortModel->setSourceModel(_model);

	ui->filesInZipList->setModel(_sortModel);
	ui->filesInZipList->hideColumn(1);
	ui->filesInZipList->hideColumn(2);
	ui->filesInZipList->hideColumn(3);
	ui->filesInZipList->header()->setStretchLastSection(true);
	ui->filesInZipList->header()->setSortIndicator(1, Qt::AscendingOrder);
	ui->filesInZipList->setSortingEnabled(true);
}

void InstallModWindow::clearInstallDirectory(bool mk){
	_installDir.removeRecursively();
	if(mk) _installDir.mkdir(".");
}

void InstallModWindow::addMod(){

	QPersistentModelIndex rootIndex = _sortModel->mapFromSource(_model->index(_installDir.absolutePath()));

	if(_sortModel->data(rootIndex, Qt::CheckStateRole) == Qt::Unchecked) return;

	QStack<QPersistentModelIndex> toTreat;
	toTreat.push(rootIndex);

	while(!toTreat.isEmpty()){
		auto index = toTreat.pop();

		for(int i = 0; i < _sortModel->rowCount(index); ++i){
			QPersistentModelIndex child = _sortModel->index(i, 0, index);
			if(_sortModel->data(child, Qt::CheckStateRole) != Qt::Unchecked){
				bool isDir = _model->isDir(_sortModel->mapToSource(child));
				QFileInfo const path = _model->filePath(_sortModel->mapToSource(child));
				if(isDir && _sortModel->data(child, Qt::CheckStateRole) == Qt::Checked){
					copyDir(path.absoluteFilePath(), _scriptsDir.absoluteFilePath(path.fileName()));
				}else if(!isDir){
					if(path.fileName().endsWith(".asi") || _type == ASI){
						QFile::copy(path.absoluteFilePath(),  _modsDir.absoluteFilePath(path.fileName()));
						if(path.fileName().endsWith(".asi")){
							emit modAdded(path.fileName());
						}
					}else if(path.fileName().endsWith(".dll") || _type == DLL){
						QFile::copy(path.absoluteFilePath(),  _scriptsDir.absoluteFilePath(path.fileName()));
						if(path.fileName().endsWith(".dll")){
							emit modAdded(path.fileName());
						}
					}else{
						//TODO
					}
				}else{
					toTreat.push(child);
				}
			}
		}

	}


	QMessageBox::information(this, tr("Success"), tr("Your mod has been installed with success"), QMessageBox::Ok);
	hide();
}

void InstallModWindow::copyDir(const QDir &from, const QDir &to){
	QFileInfo f{from.absolutePath()};
	if(!f.exists() || !f.isDir()) return;

	QStack<QString> stack;

	// Add Parent to the stack
	stack.push(from.absolutePath());

	while(!stack.isEmpty()){
		auto file = QFileInfo{stack.pop()};
		if(!file.isDir()){
			QString absolute = file.absolutePath();
			QString relative = absolute.right(absolute.length() - from.absolutePath().length());
			to.mkpath(relative.isEmpty() ? "." : relative);
			QFile::copy(file.absoluteFilePath(), to.absolutePath() + relative + "/" + file.fileName());
		}else{
			QFileInfoList files = QDir{file.filePath()}.entryInfoList(QStringList() << "*", QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
			for(auto const& f : files){
				stack.push(f.absoluteFilePath());
			}
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

QSet<QString> InstallModWindow::detectNeededFiles(QDir _installDir, modsStruct detectedMods, bool takeAllConfigFiles){
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

	QSet<QString> files;
	for(QString const& mod : detectedMods._detectedAsi){
		files.insert(mod + ".asi");
	}

	for(QString const& mod : detectedMods._detectedDll){
		files.insert(mod + ".dll");
	}

	QFileInfoList filesInfos = _installDir.entryInfoList(QStringList() << "*", QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	for(QFileInfo const& fileInfos : filesInfos){
		QString const file = fileInfos.fileName();
		bool isFile = fileInfos.isFile();
		// No need of these files
		if(isFile && (file == "unzip.exe" || file == "unrar.exe" || file == "mod.zip" || file == "mod.rar")) continue;

		// File is either a raw mod or a config file
		if(isFile && takeAllConfigFiles && (file.endsWith(".asi") || file.endsWith(".dll") || file.endsWith(".xml") || file.endsWith(".ini"))){
			files.insert(fileInfos.absoluteFilePath());
		} else if(!isFile && (detectedMods._detectedDll.contains(file))){
			files.insert(fileInfos.absoluteFilePath());
		}else if(takeAllConfigFiles){
			// If the file also starts with the name of a mod file, take it
			for(auto const& mod : detectedMods._detectedAsi + detectedMods._detectedDll){
				if(file.startsWith(mod)){
					files.insert(fileInfos.absoluteFilePath());
				}
			}
		}
	}
	return files;
}

void InstallModWindow::validateEdit(QString const& text){
	QFile f{text};
	if(!f.exists()) return;

	initFileSystemModel();

	if(text.endsWith(".zip") || text.endsWith(".rar")){
		_currentDir = _installDir;
		// Remove the install dir & re-create it
		clearInstallDirectory();
		// Copy unzip & the mod
		copyAndExtractZip(text);

		modsStruct detectedMods{detectModFiles()};


		QPersistentModelIndex rootIndex = _model->setRootPath(_currentDir.absolutePath());

		// Wait for model to load
		QEventLoop loop;
		connect(_model, &QCheckableFileSystemModel::directoryLoaded, &loop, &QEventLoop::quit);
		loop.exec();


		QStack<QPersistentModelIndex> toProcess;
		toProcess.push(rootIndex);

		while(!toProcess.empty()){
			auto toTreat = toProcess.pop();
			QSet<QString> neededFiles = detectNeededFiles(
						_model->filePath(toTreat),
						detectedMods,
						true
			);
			for(int i = 0; i < _model->rowCount(toTreat); ++i){
				auto child = _model->index(i, 0, toTreat);
				auto sortModelIndex = _sortModel->mapFromSource(child);
				if(!sortModelIndex.isValid()) continue;
				QString p = _model->filePath(child);
				if(neededFiles.contains(p)){
					_model->setData(child, Qt::Checked, Qt::CheckStateRole);
				}
				if(_model->isDir(child)){
					_model->discover(child);
					toProcess.push(child);
				}

			}
		}


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


		QStringList allAcceptedDirs;
		allAcceptedDirs << mods._detectedAsi << mods._detectedDll;
		for(QString acceptedDir : allAcceptedDirs){
			allAcceptedDirs << (acceptedDir + ".ini");
			allAcceptedDirs << (acceptedDir + ".xml");
			allAcceptedDirs << (acceptedDir + ".dll");
			allAcceptedDirs << (acceptedDir + ".asi");
		}
		_sortModel->setAcceptedDirs(allAcceptedDirs);
		_model->setNameFilters(QStringList{});
		QPersistentModelIndex rootIndex = _sortModel->mapFromSource(_model->setRootPath(_currentDir.absolutePath()));

		// Wait for model to load
		QEventLoop loop;
		connect(_model, &QCheckableFileSystemModel::directoryLoaded, &loop, &QEventLoop::quit);
		loop.exec();


		for(int i = 0; i < _sortModel->rowCount(rootIndex); ++i){
			QPersistentModelIndex child = _sortModel->index(i, 0, rootIndex);
			_sortModel->setData(child, Qt::Checked, Qt::CheckStateRole);
		}

	}

	ui->filesInZipList->setRootIndex(_sortModel->mapFromSource(_model->index(_currentDir.absolutePath())));
	ui->filesInZipList->setVisible(true);
	ui->validateButton->setVisible(true);
	setFixedSize(sizeHint());
}

void InstallModWindow::hideEvent(QHideEvent *e){
	e->accept();
	delete _model;
	clearInstallDirectory(false);
	deleteLater();
}

InstallModWindow::~InstallModWindow(){
	delete ui;
}
