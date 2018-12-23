#ifndef THREADEDPROGRESSBAR_H
#define THREADEDPROGRESSBAR_H

#include "SelfDeleteDialog.h"
#include <QWidget>
#include <QMutex>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>

class ThreadedProgressBar : public SelfDeleteDialog
{
	public:
		ThreadedProgressBar(QWidget *parent = nullptr);
		inline void setMax(qint64 max) { _max = max; }
		inline void setLabel(QString const& value) { _label.setText(value); }
		inline qint64 getMax() const { return _max; }
		void add(quint64 value);
	private:
		qint64 _max;
		qint64 _current = 0;
		QVBoxLayout _layout;
		QLabel _label;
		QProgressBar _progressBar;
		static QMutex s_mutex;
	protected:
		virtual void retranslateUi() override;
};

#endif // THREADEDPROGRESSBAR_H
