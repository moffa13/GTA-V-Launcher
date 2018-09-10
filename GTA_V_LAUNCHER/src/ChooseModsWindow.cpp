#include "ChooseModsWindow.h"
#include "MainWindow.h"
#include <QFont>
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QAbstractButton>
#include "Utilities.h"
#include "ui_ChooseModsWindow.h"

ChooseModsWindow::ChooseModsWindow(QWidget *parent) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint), ui(new Ui::ChooseModsWindow){
	ui->setupUi(this);
	init();
	getFromFiles();
	connectAll();
}

ChooseModsWindow::~ChooseModsWindow(){
	delete ui;
}

void ChooseModsWindow::init(){

	setWindowTitle(tr("Select active mods"));
	setFixedSize(690, 500);

	setStyleSheet("#ChooseModsWindow>QPushButton{"
					"padding: 10px;"
					"font-size: 12px;"
				  "}"
	);

	ui->listViewEnabled->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->listViewDisabled->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui->enableAllButton->setText(tr("Enable all"));
	ui->enableAllButton->setEnabled(false);
	ui->disableAllButton->setText(tr("Disable all"));
	ui->disableAllButton->setEnabled(false);

	ui->installModButton->setText(tr("Install a mod"));

	ui->setEnabledButton->setText(tr("Set enabled"));
	ui->setEnabledButton->setIcon(QIcon(":/images/upArrow.png"));
	ui->setEnabledButton->setEnabled(false);

	ui->deleteModButton->setText(tr("Delete mod"));
	ui->deleteModButton->setEnabled(false);

	ui->setDisabledButton->setText(tr("Set disabled"));
	ui->setDisabledButton->setIcon(QIcon(":/images/downArrow.png"));
	ui->setDisabledButton->setEnabled(false);

	ui->resetConfigButton->setText(tr("Reset from config"));
	ui->validateButton->setText(tr("Confirm"));
}

void ChooseModsWindow::connectAll(){
	connect(ui->validateButton, SIGNAL(clicked()), this, SLOT(setModsSlot()));
	connect(ui->listViewEnabled, SIGNAL(clicked(QModelIndex)), this, SLOT(setButtonUpDownSlot(QModelIndex)));
	connect(ui->listViewDisabled, SIGNAL(clicked(QModelIndex)), this, SLOT(setButtonUpDownSlot(QModelIndex)));
	connect(ui->setEnabledButton, &QPushButton::clicked, [this](){
		enableDisableMod(m_modele1);
	});
	connect(ui->setDisabledButton, &QPushButton::clicked, [this](){
		enableDisableMod(m_modele2);
	});

	connect(ui->installModButton, &QPushButton::clicked, [this](){
		m_installModWindow = new InstallModWindow(MainWindow::m_gtaDirectoryStr + "/installMod", MainWindow::m_gtaDirectoryStr, MainWindow::m_gtaDirectoryStr + "/scripts", this);
		connect(m_installModWindow, &InstallModWindow::modAdded, [this](QString const& file){
			QStringList elem{file};
			auto v = addVersionToElements(MainWindow::m_gtaDirectoryStr, MainWindow::m_gtaDirectoryStr + "/scripts", elem);
			m_enabledModsAndVersions.append(v.first());
			m_modele1->insertRow(m_modele1->rowCount());
			m_modele1->setData(m_modele1->index(m_modele1->rowCount() - 1), toQStringList(v, true).first());
		});
		m_installModWindow->exec();
	});

	connect(ui->enableAllButton, &QPushButton::clicked, [this](){
		QStringList all = toQStringList(m_enabledModsAndVersions);
		all.append(toQStringList(m_disabledModsAndVersions));

		m_enabledModsAndVersions = addVersionToElements(MainWindow::m_gtaDirectoryStr, MainWindow::m_gtaDirectoryStr + "/scripts", all);
		m_disabledModsAndVersions.clear();

		setEnabledModsToList(m_enabledModsAndVersions);
		setDisabledModsToList(m_disabledModsAndVersions);
		setEnableDisableAllButtons();
	});

	connect(ui->disableAllButton, &QPushButton::clicked, [this](){
		QStringList all = toQStringList(m_enabledModsAndVersions);
		all.append(toQStringList(m_disabledModsAndVersions));

		m_disabledModsAndVersions = addVersionToElements(MainWindow::m_gtaDirectoryStr, all);
		m_enabledModsAndVersions.clear();

		setEnabledModsToList(m_enabledModsAndVersions);
		setDisabledModsToList(m_disabledModsAndVersions);
		setEnableDisableAllButtons();
	});

	connect(ui->deleteModButton, SIGNAL(clicked()), this, SLOT(deleteModSlot()));
	connect(ui->resetConfigButton, SIGNAL(clicked()), this, SLOT(loadConfigSlot()));
}

QString ChooseModsWindow::basePathFromModType(QString const& mod){
	if(mod.endsWith(".dll")){
		return MainWindow::m_gtaDirectoryStr + "/scripts";
	}else if(mod.endsWith(".asi")){
		return MainWindow::m_gtaDirectoryStr;
	}
	return QString{};
}

void ChooseModsWindow::hideEvent(QHideEvent *e){
	e->accept();
	deleteLater();
}

void ChooseModsWindow::getFromFiles(){

	// Get from files
	QStringList enabledMods = getEnabledModsFromFiles();
	QStringList disabledMods = getDisabledModsFromFiles();

	// Remove files Conflicts between disabled and enabled mods
	noConflicts(enabledMods, disabledMods);

	m_enabledModsAndVersions = addVersionToElements(MainWindow::m_gtaDirectoryStr, MainWindow::m_gtaDirectoryStr + "/scripts", enabledMods);
	m_disabledModsAndVersions = addVersionToElements(MainWindow::m_disabledModsDirectoryStr, disabledMods);

	// Set to list
	setEnabledModsToList(m_enabledModsAndVersions);
	setDisabledModsToList(m_disabledModsAndVersions);
}

/**
 * Feeds the list view with list of items
 * @brief ChooseModsWindow::setEnabledModsToList
 * @param enabledMods
 */
void ChooseModsWindow::setEnabledModsToList(const QList<QPair<QString, Version>> &enabledMods){
	m_modele1 = new QStringListModel(toQStringList(enabledMods, true), this);
	m_modele1->setObjectName("enabled");
	ui->listViewEnabled->setModel(m_modele1);
	setEnableDisableAllButtons();
}

/**
 * Feeds the list view with list of items
 * @brief ChooseModsWindow::setDisabledModsToList
 * @param disabledMods
 */
void ChooseModsWindow::setDisabledModsToList(const QList<QPair<QString, Version>> &disabledMods){
	m_modele2 = new QStringListModel(toQStringList(disabledMods, true), this);
	m_modele2->setObjectName("disabled");
	ui->listViewDisabled->setModel(m_modele2);
	setEnableDisableAllButtons();
}

QStringList ChooseModsWindow::getEnabledModsFromFiles(){
	QStringList asiFilter("*.asi");
	QStringList dllFilter("*.dll");
	QDir enabledModsDirectory(MainWindow::m_gtaDirectoryStr);
	QDir enabledDllModsDirectory(MainWindow::m_gtaDirectoryStr + "/scripts");
	QStringList enabledMods = enabledModsDirectory.entryList(asiFilter, QDir::Files);
	enabledMods.append(enabledDllModsDirectory.entryList(dllFilter, QDir::Files));
	return enabledMods;
}

QStringList ChooseModsWindow::getDisabledModsFromFiles(){
	QStringList disabledModsFilter{};
	disabledModsFilter << "*.asi" << "*.dll";
	QDir disabledModsDirectory(MainWindow::m_disabledModsDirectoryStr);
	QStringList disabledMods = disabledModsDirectory.entryList(disabledModsFilter, QDir::Files);
	return disabledMods;
}

/**
 * Takes a string list and return a list containing a pair (name, version)
 * @brief ChooseModsWindow::addVersionToElements
 * @param base
 * @param list
 * @return
 */
QList<QPair<QString, Version>> ChooseModsWindow::addVersionToElements(QString const& baseAsi, QString const& baseDll, QStringList const& list) const{
	QList<QPair<QString, Version>> listWithVersions;
	for(QString const& elem : list){
		Version v;
		if(elem.endsWith(".dll")){
			v = Utilities::getFileVersion(baseDll + "/" + elem);
		}else{
			v = Utilities::getFileVersion(baseAsi + "/" + elem);
		}
		listWithVersions.append(QPair<QString, Version>(elem, v));
	}
	return listWithVersions;
}

QList<QPair<QString, Version>> ChooseModsWindow::addVersionToElements(QString const& base, QStringList const& list) const{
	return addVersionToElements(base, base, list);
}

QStringList ChooseModsWindow::toQStringList(const QList<QPair<QString, Version>> &list, bool addVersion){
	QStringList ret;
	for(QPair<QString, Version> const& elem : list){
		QString val = elem.first;
		if(addVersion && elem.second.getVersionInt() != 0)
			val += " V" + QString{elem.second.getVersionStr().c_str()};
		ret.append(val);
	}
	return ret;
}

void ChooseModsWindow::noConflicts(QStringList &enableMods, QStringList &disabledMods){
	QSet<QString> enabledModsSet = enableMods.toSet();
	QSet<QString> disabledModsSet = disabledMods.toSet();

	QSet<QString> enabledModsSub = enabledModsSet - disabledModsSet;
	QSet<QString> c1 = enabledModsSet - enabledModsSub;
	noConflicts(enableMods, disabledMods, c1);
}

void ChooseModsWindow::noConflicts(QStringList &enableMods, QStringList &disabledMods, QSet<QString> const& conflicts){
	foreach(const QString &file, conflicts){
		QMessageBox msg;
		msg.setWindowTitle(tr("Conflict"));
		msg.setText(tr("There is a conflict with ") + file);
		QAbstractButton *keepEnabled = msg.addButton(tr("Keep mod in enabled mods"), QMessageBox::YesRole);
		msg.addButton(tr("Keep mod in disabled mods"), QMessageBox::NoRole);
		msg.setIcon(QMessageBox::Warning);
		msg.exec();
		const QString *deletedPath;
		if(msg.clickedButton() == keepEnabled){
			deletedPath = &MainWindow::m_disabledModsDirectoryStr;
			disabledMods.removeOne(file);
		}else{
			deletedPath = &MainWindow::m_gtaDirectoryStr;
			enableMods.removeOne(file);
		}
		QFile::remove(*deletedPath + "/" + file);
	}
}

void ChooseModsWindow::enableButton(QString btn){
	if(btn == "enabled"){
		ui->setEnabledButton->setEnabled(false);
		ui->setDisabledButton->setEnabled(true);
	}else{
		ui->setEnabledButton->setEnabled(true);
		ui->setDisabledButton->setEnabled(false);
	}
}

void ChooseModsWindow::enableDisableMod(QStringListModel *model){
	if(m_lastIndex.data().toString().isEmpty()) return;
	QString filename = m_lastIndex.data().toString();
	QStringListModel *selectedModel;
	QString what = model->objectName();
	if(what == "enabled"){
		auto elem = m_disabledModsAndVersions.takeAt(m_lastIndex.row());
		m_enabledModsAndVersions.append(elem);
		m_modele2->removeRow(m_lastIndex.row());
		m_modele1->insertRow(m_modele1->rowCount());
		selectedModel = m_modele1;
	}else{
		auto elem = m_enabledModsAndVersions.takeAt(m_lastIndex.row());
		m_disabledModsAndVersions.append(elem);
		m_modele1->removeRow(m_lastIndex.row());
		m_modele2->insertRow(m_modele2->rowCount());
		selectedModel = m_modele2;
	}
	QModelIndex index = selectedModel->index(selectedModel->rowCount()-1);
	selectedModel->setData(index, filename);
	setEnableDisableAllButtons();
}

void ChooseModsWindow::disableAllMods(){
	QStringList en = getEnabledModsFromFiles();
	QStringList di = getDisabledModsFromFiles();
	noConflicts(en, di);
	saveMods(en);
}

void ChooseModsWindow::enableAllMods(){
	QStringList en = getEnabledModsFromFiles();
	QStringList di = getDisabledModsFromFiles();
	noConflicts(en, di);
	saveMods(QStringList(), di);
}

void ChooseModsWindow::enableOldConfig(){
	auto pair = getModsConfig();
	saveMods(pair.second, pair.first);
}

QStringList ChooseModsWindow::getEnabledModsFromList(){
	return toQStringList(m_enabledModsAndVersions);
}

QStringList ChooseModsWindow::getDisabledModsFromList(){
	return toQStringList(m_disabledModsAndVersions);
}

QStringList ChooseModsWindow::checkModsExists(QStringList &list){
	QStringList duplicates;
	for(int i=0; i<list.length(); i++){
		const QString &filename = list.at(i);
		QFileInfo info(MainWindow::m_gtaDirectoryStr + "/" + filename);
		QFileInfo info2(MainWindow::m_disabledModsDirectoryStr + "/" + filename);
		if(!info.exists() && !info2.exists()){
			list.removeAt(i);
		}else if(info.exists() && info2.exists()){
			duplicates << filename;
		}
	}
	return duplicates;
}

void ChooseModsWindow::setModsSlot(){
	int resp = QMessageBox::information(this, tr("Sure ?"), tr("Are you sure ?"), QMessageBox::Yes | QMessageBox::No);
	setConfigSlot();
	if(resp == QMessageBox::Yes){
		saveMods(m_enabledModsAndVersions, m_disabledModsAndVersions);
	}else{
		return;
	}
	resp = QMessageBox::information(this, tr("Play"), tr("Do you want to play now ?"), QMessageBox::Yes | QMessageBox::No);
	if(resp == QMessageBox::Yes){
		MainWindow *parent = qobject_cast<MainWindow*>(this->parent());
		parent->startGtaWithModsSlot();
	}
	this->close();
}

QPair<QStringList, QStringList> ChooseModsWindow::getModsConfig(){

	// Add Current mods to the list
	QStringList list = getEnabledModsFromFiles();
	QStringList list2 = getDisabledModsFromFiles();

	// Fetch from config
	QStringList enabledFromConfig = Utilities::removeValues(Utilities::loadFromConfig("EnabledMods"));
	checkModsExists(enabledFromConfig);
	for(QString const& enabled : enabledFromConfig){
		if(list2.contains(enabled)){
			list2.removeOne(enabled);
		}
		list.append(enabled);
	}

	QStringList disabledFromConfig = Utilities::removeValues(Utilities::loadFromConfig("DisabledMods"));
	checkModsExists(disabledFromConfig);
	for(QString const& disabled : disabledFromConfig){
		if(list.contains(disabled)){
			list.removeOne(disabled);
		}
		list2.append(disabled);
	}

	list.removeDuplicates();
	list2.removeDuplicates();

	return QPair<QStringList, QStringList>(list, list2);
}

void ChooseModsWindow::loadConfigSlot(){

	auto pair = getModsConfig();

	m_enabledModsAndVersions = addVersionToElements(MainWindow::m_gtaDirectoryStr, MainWindow::m_gtaDirectoryStr + "/scripts", pair.first);
	m_disabledModsAndVersions = addVersionToElements(MainWindow::m_disabledModsDirectoryStr, pair.second);

	setEnabledModsToList(m_enabledModsAndVersions);
	setDisabledModsToList(m_disabledModsAndVersions);
}

void ChooseModsWindow::setConfigSlot(){
	QSettings settings("Bigcoding", "GTA V Launcher");
	settings.beginGroup("DisabledMods");
	settings.remove("");
	settings.endGroup();
	settings.beginGroup("EnabledMods");
	settings.remove("");
	settings.endGroup();
	qDebug() << getDisabledModsFromList();
	Utilities::setToConfig("EnabledMods", getEnabledModsFromList());
	Utilities::setToConfig("DisabledMods", getDisabledModsFromList());
}

void ChooseModsWindow::setButtonUpDownSlot(QModelIndex index){
	enableButton(index.model()->objectName());
	ui->deleteModButton->setEnabled(true);
	m_lastIndex = index;
}

void ChooseModsWindow::saveMods(QList<QPair<QString, Version>> const& enabledModsAndVersions, const QList<QPair<QString, Version> > &disabledModsAndVersions){
	saveMods(toQStringList(disabledModsAndVersions), toQStringList(enabledModsAndVersions));
}

void ChooseModsWindow::saveMods(QStringList const& disable, QStringList const& enable){
	for(const QString &file : disable){
		bool copied = QFile::copy(basePathFromModType(file) + "/" + file, MainWindow::m_disabledModsDirectoryStr + "/" + file);
		// Ensure mod has been copied to avoid something we don't want
		if(copied) QFile::remove(basePathFromModType(file) + "/" + file);
	}
	for(const QString &file : enable){
		bool copied = QFile::copy(MainWindow::m_disabledModsDirectoryStr + "/" + file, basePathFromModType(file) + "/" + file);
		// Ensure mod has been copied to avoid something we don't want
		if(copied) QFile::remove(MainWindow::m_disabledModsDirectoryStr + "/" + file);
	}
}

void ChooseModsWindow::deleteModSlot(){
	if(m_lastIndex.data().toString().isEmpty()) return;
	int resp = QMessageBox::information(this, tr("Delete mod"), tr("Are you sure ?"), QMessageBox::Yes | QMessageBox::No);
	if(resp == QMessageBox::Yes){
		QString filename =  m_lastIndex.model()->objectName() == "enabled"
				? m_enabledModsAndVersions.at(m_lastIndex.row()).first
				: m_disabledModsAndVersions.at(m_lastIndex.row()).first;
		QString const path = basePathFromModType(filename);
		QStringListModel *model = m_lastIndex.model()->objectName() == "enabled" ? m_modele1 : m_modele2;
		QFile::remove(path + "/" + filename);
		QFile::remove(MainWindow::m_disabledModsDirectoryStr + "/" + filename);
		model->removeRow(m_lastIndex.row());
		if(m_lastIndex.model()->objectName() == "enabled"){
			m_enabledModsAndVersions.removeAt(m_lastIndex.row());
		}else{
			m_disabledModsAndVersions.removeAt(m_lastIndex.row());
		}
		setEnableDisableAllButtons();
	}
}

void ChooseModsWindow::setEnableDisableAllButtons(){
	if(!m_disabledModsAndVersions.empty()){
		ui->enableAllButton->setEnabled(true);
	}else{
		ui->enableAllButton->setEnabled(false);
	}

	if(!m_enabledModsAndVersions.empty()){
		ui->disableAllButton->setEnabled(true);
	}else{
		ui->disableAllButton->setEnabled(false);
	}
}
