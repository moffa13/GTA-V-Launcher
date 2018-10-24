#include "Version.h"
#include <QRegExp>
#include <QtDebug>

Version::Version(unsigned major, unsigned minor, unsigned patch, unsigned more) : _major{major}, _minor{minor}, _patch{patch}, _more{more}{}

Version::Version(QString const& version){

	if(version.isEmpty()){
		_major = 0;
		_minor = 0;
		_patch = 0;
		_more = 0;
		return;
	}

	QRegExp version_regex{"^(([\\d]+)(?:\\.([\\d]+))?(?:\\.([\\d]+))?(?:\\.([\\d]+))?)$"};
	version_regex.indexIn(version, 0);

	if(version_regex.cap(0).isEmpty()){
		throw std::runtime_error{"Unknown version format"};
	}

	_major = version_regex.cap(2).isEmpty() ? 0 : version_regex.cap(2).toUInt();
	_minor = version_regex.cap(3).isEmpty() ? 0 : version_regex.cap(3).toUInt();
	_patch = version_regex.cap(4).isEmpty() ? 0 : version_regex.cap(4).toUInt();
	_more = version_regex.cap(5).isEmpty() ? 0 : version_regex.cap(5).toUInt();

}

unsigned Version::getMajor() const{
	return _major;
}

unsigned Version::getMinor() const{
	return _minor;
}

unsigned Version::getPatch() const{
	return _patch;
}

unsigned Version::getMore() const{
	return _more;
}

std::string Version::getVersionStr(char delimiter) const{
	return std::string{std::to_string(_major)} + delimiter + std::to_string(_minor) + delimiter + std::to_string(_patch) + delimiter + std::to_string(_more);
}

unsigned Version::getVersionInt() const{
	return _major * 1000 + _minor * 100 + _patch * 10 + _more;
}

bool Version::operator <(const Version &lhs) const{
	return getVersionInt() < lhs.getVersionInt();
}

bool Version::operator >(const Version &lhs) const{
	return !operator <(lhs) && operator !=(lhs);
}

bool Version::operator >=(const Version &lhs) const{
	return !operator<(lhs);
}

bool Version::operator <=(const Version &lhs) const{
	return !operator>(lhs);
}


bool Version::operator ==(const Version &lhs) const{
	return _major == lhs._major && _minor == lhs._minor && _patch == lhs._patch && _more == lhs._more;
}

bool Version::operator !=(const Version &lhs) const{
	return !operator==(lhs);
}


