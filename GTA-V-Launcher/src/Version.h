#ifndef VERSION_H
#define VERSION_H

#include <QString>

class Version
{
	public:
		Version(unsigned major = 0, unsigned minor = 0, unsigned patch = 0, unsigned more = 0);
		Version(QString const& version);
		bool operator <(Version const& lhs) const;
		bool operator >(Version const& lhs) const;
		bool operator <=(Version const& lhs) const;
		bool operator >=(Version const& lhs) const;
		bool operator ==(Version const& lhs) const;
		bool operator !=(Version const& lhs) const;
		unsigned getVersionInt() const;
		unsigned getMore() const;
		unsigned getPatch() const;
		unsigned getMinor() const;
		unsigned getMajor() const;
		std::string getVersionStr(char delimiter = '.') const;
	private:
		unsigned _major;
		unsigned _minor;
		unsigned _patch;
		unsigned _more;
};

#endif // VERSION_H
