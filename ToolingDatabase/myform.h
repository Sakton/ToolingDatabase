#pragma once

#include <QWidget>
#include <QDataWidgetMapper>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include "mysqltablemodel.h"

namespace Ui
{
  class MyForm;
}

class MyForm : public QWidget
{
  Q_OBJECT

public:
  explicit MyForm(QWidget *parent = 0);

public:
  void setMyModel(MySqlTableModel *myModel);
  void setMyProxyModel(QSortFilterProxyModel *proxy);
  QDataWidgetMapper *getMyMapper();

private slots:
  void on_pushButton_Save_clicked();
  void on_pushButton_Cancel_clicked();

private:
  Ui::MyForm *ui;
  QDataWidgetMapper *myMapWidget;
  void initForm(QSortFilterProxyModel *proxy);
  void initForm(MySqlTableModel *myModel);
  void init();
};
