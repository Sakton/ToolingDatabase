#pragma once

#include <QSqlTableModel>
#include <QColor>
#include <QDate>

class MySqlTableModel : public QSqlTableModel
{
public:
  MySqlTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
public:
  QVariant data(const QModelIndex &index, int role) const;
};
