#include "ThreadedProgressBar.h"

#include <QtDebug>

QMutex ThreadedProgressBar::s_mutex;

ThreadedProgressBar::ThreadedProgressBar(QWidget *parent) : SelfDeleteDialog(parent){
	setLayout(&_layout);
	_layout.addWidget(&_label);
	_layout.addWidget(&_progressBar);

	_progressBar.setMaximum(1000);
	_progressBar.setValue(0);

}

void ThreadedProgressBar::add(quint64 value){
	QMutexLocker locker(&s_mutex);

	_current += value;

	if(_current == 0){
		_progressBar.setValue(0);
		return;
	}

	int ratio = static_cast<int>(1000 / (static_cast<float>(_max) / _current));

	_progressBar.setValue(ratio);
}

void ThreadedProgressBar::retranslateUi(){
	// Nothing to translate
}



