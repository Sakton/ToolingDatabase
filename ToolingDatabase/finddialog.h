#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui
{
  class FindDialog;
}

class FindDialog : public QDialog
{
  Q_OBJECT

public:
  explicit FindDialog(QWidget *parent = nullptr);
  ~FindDialog();

public:
  QString getTextSearch();

private slots:
  void on_pushSearchBtn_clicked();
  void closeEvent(QCloseEvent *event);
  void on_pushCancelBtn_clicked();

signals:
  void closeFindDialogSignal();
  void sendFindTextFromFindDialog(const QString &findText);

private:
  Ui::FindDialog *ui;

private:
  QString textSearch;
};

#endif // FINDDIALOG_H
