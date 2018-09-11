#ifndef DIRSINFOS_H
#define DIRSINFOS_H

#include <QString>

class DirsInfos
{
	public:
		DirsInfos(QString const& dir);
	private:
		void add(QString const& name, bool directory);
		void iterateThroughDir();
};

#endif // DIRSINFOS_H
