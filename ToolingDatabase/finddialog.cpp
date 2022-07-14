#include "finddialog.h"

#include <QDebug>

#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) : QDialog(parent), ui(new Ui::FindDialog) {
  ui->setupUi(this);
  this->setWindowTitle(tr("ПОИСК"));
}

FindDialog::~FindDialog() {
  delete ui;
}

QString FindDialog::getTextSearch() {
  return this->textSearch;
}

void FindDialog::on_pushSearchBtn_clicked() {
  this->textSearch = this->ui->findTextLine->text();
  if (textSearch.isEmpty()) {
	this->hide();
	QMessageBox::StandardButton ret;
	ret = QMessageBox::warning(this, tr("ОШИБКА"), tr("Надо ввести строку поиска"),
							   QMessageBox::Ok | QMessageBox::Close);
	switch (ret) {
	  case QMessageBox::Ok:
		this->show();
		break;
	  case QMessageBox::Close:
		this->close();
		break;
	  default:
		break;
	}
  }
  emit sendFindTextFromFindDialog(this->textSearch);
  this->close();
}

void FindDialog::closeEvent(QCloseEvent *event) {
  if (this->close()) {
	emit closeFindDialogSignal();  // сигнал при нажатии на закрытие окошка
	event->accept();
  } else {
	event->ignore();
  }
}

void FindDialog::on_pushCancelBtn_clicked() {
  emit closeFindDialogSignal();	 // сигнал при нажатии на кпопку отмена
  delete this;
}
