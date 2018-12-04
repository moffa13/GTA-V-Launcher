#ifndef SELFDELETEDIALOG_H
#define SELFDELETEDIALOG_H

#include <QDialog>
#include <QHideEvent>

class SelfDeleteDialog : public QDialog
{
		Q_OBJECT

	public:
		SelfDeleteDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	protected:
		void hideEvent(QHideEvent *event) override;
	Q_SIGNALS:
		void hidden();
};

#endif // SELFDELETEDIALOG_H
