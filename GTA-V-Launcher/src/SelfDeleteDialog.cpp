#include "SelfDeleteDialog.h"

SelfDeleteDialog::SelfDeleteDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f){}

void SelfDeleteDialog::hideEvent(QHideEvent *e){
	e->accept();
	emit hidden();
	deleteLater();
}
