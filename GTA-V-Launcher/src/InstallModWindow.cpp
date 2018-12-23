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
#include "ChooseModsWindow.h"

InstallModWindow::InstallModWindow(QString const& installDir, QString const& modsDir, QString const& scriptsDir, QWidget *parent) :
	SelfDeleteDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
	ui(new Ui::InstallModWindow),
	_modsDir{modsDir},
	_scriptsDir{scriptsDir},
	_installDir{installDir}
{
	ui->setupUi(this);
	ui->modFileEdit->setPlaceholderText("C:\\Users\\GTAV\\Downloads\\mod.zip");

	connect(ui->selectFileButton, &QPushButton::clicked, [this](){
		ui->modFileEdit->clear();
		QString const file = QFileDialog::getOpenFileName(
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
	delete _sortModel;
	_model = nullptr;
	_sortModel = nullptr;
	ui->filesInZipList->setModel(nullptr);


	_model = new QCheckableFileSystemModel(this);

	connect(_model, &QCheckableFileSystemModel::dataChanged, [this](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()){
		if(!roles.isEmpty() && !roles.contains(Qt::CheckStateRole)) return; // no checkboxes modified

		QModelIndex toTreat = topLeft;
		if(topLeft != bottomRight){ // one element, treat only if a dir
			 toTreat = topLeft.parent();
		}

		if(!_model->isDir(toTreat) || !_sortModel->mapFromSource(toTreat).isValid()) return;

		ModType curDirType = NONE;
		for(int i = 0; i < _model->rowCount(toTreat); ++i){
			QPersistentModelIndex child{_model->index(i, 0, toTreat)};
			if(!_sortModel->mapFromSource(child).isValid() || _model->data(child, Qt::CheckStateRole) == Qt::Unchecked) continue;
			QFileInfo f{_model->filePath(child)};
			if(!_model->isDir(child)){
				if(f.fileName().endsWith(".asi")){
					curDirType |= ASI;
				}else if(f.fileName().endsWith(".dll")){
					curDirType |= DLL;
				}
			}
		}
		_type[_model->filePath(toTreat)] = curDirType;

	});

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

//	for(QHash<QString, ModType>::iterator it = _type.begin(); it != _type.end(); ++it){
//		qDebug() << it.key() << " = " << it.value();
//	}

	QPersistentModelIndex rootIndex = _model->index(_currentDir.absolutePath());

	// If first element is unchecked, we can abort
	if(!_sortModel->mapFromSource(rootIndex).isValid() || _model->data(rootIndex, Qt::CheckStateRole) == Qt::Unchecked) return;

	QStack<QPersistentModelIndex> toTreat;
	toTreat.push(rootIndex);

	while(!toTreat.isEmpty()){

		const auto index = toTreat.pop();

		for(int i = 0; i < _model->rowCount(index); ++i){
			if(!_sortModel->mapFromSource(index).isValid()) continue;
			QPersistentModelIndex child = _model->index(i, 0, index);
			if(_model->data(child, Qt::CheckStateRole) != Qt::Unchecked){
				bool isDir = _model->isDir(child);
				QFileInfo const path = _model->filePath(child);

				// It's a checked dir and one of its sibling files is a dll
				if(isDir && _model->data(child, Qt::CheckStateRole) == Qt::Checked && _type[_model->filePath(index)] == DLL){
					copyDir(path.absoluteFilePath(), _scriptsDir.absoluteFilePath(path.fileName()));
				}else if(!isDir){ // asi, dll, xml, ...
					ChooseModsWindow *parent = qobject_cast<ChooseModsWindow*>(this->parentWidget());
					if(path.fileName().endsWith(".asi") || _type[_model->filePath(index)] == ASI){ // It's a file and one of its sibling files is a asi
						if(!path.fileName().endsWith(".asi") || parent->addMod(path.absoluteFilePath())){
							QFile::copy(path.absoluteFilePath(),  _modsDir.absoluteFilePath(path.fileName()));
						}
					}else if(path.fileName().endsWith(".dll") ||  _type[_model->filePath(index)] == DLL){ // It's a file and one of its sibling files is a dll
						if(!path.fileName().endsWith(".dll") || parent->addMod(path.absoluteFilePath())){
							QFile::copy(path.absoluteFilePath(),  _scriptsDir.absoluteFilePath(path.fileName()));
						}
					}else{
						qDebug() << path.filePath() << "Not copied";
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

void InstallModWindow::retranslateUi(){
	ui->retranslateUi(this);
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
			QString relative = absolute.right(absolute.length() - from.absolutePath().length() - 1);
			if(relative == absolute) relative = "";
			to.mkpath(relative.isEmpty() ? "." : relative);
			QFile::copy(file.absoluteFilePath(), to.absolutePath() + "/" + relative + "/" + file.fileName());
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
modsStruct InstallModWindow::detectModFiles(QDir const& dir) const{
	// Only list files
	auto files = dir.entryList(QStringList() << "*.*", QDir::Files | QDir::NoDotAndDotDot);
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
	QSet<QString> files;
	for(QString const& mod : detectedMods._detectedAsi){
		files.insert(_installDir.absoluteFilePath(mod + ".asi"));
	}

	for(QString const& mod : detectedMods._detectedDll){
		files.insert(_installDir.absoluteFilePath(mod + ".dll"));
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

	QPersistentModelIndex rootIndex;

	if(text.endsWith(".zip") || text.endsWith(".rar")){
		_currentDir = _installDir;
		// Remove the install dir & re-create it
		clearInstallDirectory();
		// Copy unzip & the mod
		copyAndExtractZip(text);

		rootIndex = _model->setRootPath(_currentDir.absolutePath());


		// Wait for model to load
		QEventLoop loop;
		connect(_model, &QCheckableFileSystemModel::directoryLoaded, &loop, &QEventLoop::quit);
		loop.exec();

		QStack<QPersistentModelIndex> toProcess;
		toProcess.push(rootIndex);

		while(!toProcess.empty()){
			auto toTreat = toProcess.pop();
			modsStruct detectedMods{detectModFiles(_model->filePath(toTreat))};
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
				// If already checked, do not get in it because it's not necessary
				if(_model->isDir(child) && _model->data(child, Qt::CheckStateRole) != Qt::Checked){
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
		rootIndex = _model->setRootPath(_currentDir.absolutePath());

		// Wait for model to load
		QEventLoop loop;
		connect(_model, &QCheckableFileSystemModel::directoryLoaded, &loop, &QEventLoop::quit);
		loop.exec();


		for(int i = 0; i < _model->rowCount(rootIndex); ++i){
			QPersistentModelIndex child = _model->index(i, 0, rootIndex);
			if(_sortModel->mapFromSource(child).isValid())
				_model->setData(child, Qt::Checked, Qt::CheckStateRole);
		}

	}

	ui->filesInZipList->setRootIndex(_sortModel->mapFromSource(rootIndex));
	if(_model->rowCount(rootIndex)){
		ui->filesInZipList->setVisible(true);
		ui->validateButton->setVisible(true);
	}else{
		ui->filesInZipList->setVisible(false);
		ui->validateButton->setVisible(false);
		QMessageBox::critical(this, tr("Not found"), tr("No mod has been found, please select another file."));
	}
	setFixedSize(sizeHint());
}

InstallModWindow::~InstallModWindow(){
	clearInstallDirectory(false);
	delete _model;
	delete _sortModel;
	delete ui;
}
