#ifndef SELFDELETEDIALOG_H
#define SELFDELETEDIALOG_H

#include <QDialog>
#include <QHideEvent>

class SelfDeleteDialog : public QDialog
{
	public:
		SelfDeleteDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	protected:
		void hideEvent(QHideEvent *event) override;
};

#endif // SELFDELETEDIALOG_H
