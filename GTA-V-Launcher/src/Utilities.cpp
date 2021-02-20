#include "Utilities.h"
#include "TlHelp32.h"
#include "Psapi.h"
#include "Winbase.h"
#include <QDebug>
#include <QSettings>
#include <QFile>



Version Utilities::getFileVersion(const QString &filename){

	DWORD dwHandle;
	DWORD dwLen = GetFileVersionInfoSize(filename.toStdWString().c_str(), &dwHandle);

	LPVOID lpData = new BYTE[dwLen];
	if(!GetFileVersionInfo(filename.toStdWString().c_str(), dwHandle, dwLen, lpData))
	{
		delete[] lpData;
		return Version();
	}

	// VerQueryValue
	VS_FIXEDFILEINFO *lpBuffer = nullptr;
	UINT uLen;

	if(!VerQueryValue(lpData, QString("\\").toStdWString().c_str(), (LPVOID*)&lpBuffer, &uLen))
	{
		return Version();
	}

	QString version;

	version += QString::number((lpBuffer->dwFileVersionMS >> 16) & 0xffff) + '.';
	version += QString::number((lpBuffer->dwFileVersionMS) & 0xffff) + '.';
	version += QString::number((lpBuffer->dwFileVersionLS >> 16) & 0xffff) + '.';
	version += QString::number((lpBuffer->dwFileVersionLS) & 0xffff);

	return version;
}

bool Utilities::setProcessPriority(QString const& process, DWORD  dwPriorityClass){
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &entry) == TRUE){
		while (Process32Next(snapshot, &entry) == TRUE){
			if (std::wstring(entry.szExeFile) == process.toStdWString()){
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				BOOL response = SetPriorityClass(hProcess, dwPriorityClass);
				CloseHandle(hProcess);
				return response;
			}
		}
	}
	CloseHandle(snapshot);
	return false;
}

bool Utilities::startedAsAdmin(){
	bool fRet = FALSE;
	HANDLE hToken = nullptr;
	if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if(GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)){
			fRet = Elevation.TokenIsElevated;
		}
	}
	if(hToken) {
		CloseHandle(hToken);
	}
	return fRet;
}

QString Utilities::checkProcessRunning(QString const &name){
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE){
		while (Process32Next(snapshot, &entry) == TRUE){
			if (wcscmp(entry.szExeFile,	name.toStdWString().c_str()) == 0){
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, entry.th32ProcessID);

				WCHAR filename[MAX_PATH];
				GetModuleFileNameEx(hProcess, nullptr, filename, MAX_PATH);

//				wchar_t buf[256];
//				FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
//							  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);

				CloseHandle(hProcess);
				return QString::fromWCharArray(filename);
			}
		}
	}
	CloseHandle(snapshot);
	return nullptr;
}

std::unique_ptr<QSettings> Utilities::getSettings(){
	return std::make_unique<QSettings>("Bigcoding", "GTA V Launcher");
}

QMap<QString, QVariant> Utilities::loadFromConfig(QString const &key){
	auto settings = getSettings();
	QMap<QString, QVariant> map;
	settings->beginGroup(key);
	foreach(QString const &child, settings->childKeys()){
		map.insert(child, settings->value(child));
	}
	settings->endGroup();
	return map;
}

QVariant Utilities::loadFromConfig(QString const &key, QString const &childKey, QVariant defaultKey){
	auto settings = getSettings();
	settings->beginGroup(key);
	return settings->value(childKey, defaultKey);
}

QStringList Utilities::removeValues(QMap<QString, QVariant> const &map){
	QStringList list;
	QMapIterator<QString, QVariant> i(map);
	while(i.hasNext()){
		i.next();
		list << i.key();
	}
	qDebug() << list;
	return list;
}

void Utilities::setToConfig(QString const &key, const QStringList &data){
	auto settings = getSettings();
	settings->beginGroup(key);
	foreach(QString const &key, data){
		settings->setValue(key, QVariant(1));
	}
	settings->endGroup();
	settings->sync();
}

void Utilities::clearConfig(QString const &key){
	auto settings = getSettings();
	settings->remove(key);
	settings->sync();
}

void Utilities::setToConfig(QString const &key, const QMap<QString, QVariant> &data){
	auto settings = getSettings();
	settings->beginGroup(key);
	QMapIterator<QString, QVariant> i(data);
	while(i.hasNext()){
		i.next();
		settings->setValue(i.key(), i.value());
	}
	settings->endGroup();
	settings->sync();
}

bool Utilities::launcherCracked(){
	return Utilities::loadFromConfig("General", "LauncherCrack", false).toBool();
}

/**
 * Copies a file returns true if success, returns false if <from> do not exists
 * Returns true if <to> already exists
 * @brief Utilities::copy
 * @param from
 * @param to
 * @return
 */
bool Utilities::copy(QString const& from, QString const& to){
	QFile f{from};
	QFile f2{to};
	if(!f.exists()) return false;
	if(f2.exists()) return true;
	return QFile::copy(from, to);
}

