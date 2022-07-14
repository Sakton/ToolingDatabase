#include "mysqltablemodel.h"

MySqlTableModel::MySqlTableModel(QObject *parent, QSqlDatabase db)
	: QSqlTableModel(parent, db) {}

QVariant MySqlTableModel::data(const QModelIndex &index, int role) const {
  QVariant ind = QSqlTableModel::data(index, role);
  if (index.isValid()) {
	switch (role) {
	  case Qt::EditRole:
	  case Qt::DisplayRole: {
		if (index.column() != 8 && index.column() != 10) {
		  return ind.toString();
		} else {
		  return ind.toDate();
		}
	  }
	  case Qt::BackgroundRole:
		if ((index.column() == 11)) {
		  if ((index.data().toString() == "Выполнено")) {
			return QVariant(QColor(80, 160, 230));
		  } else if ((index.data().toString() == "Опоздание")) {
			return QVariant(QColor(Qt::red));
		  } else if ((index.data().toString() == "Отменено")) {
			return QVariant(QColor(Qt::yellow));
		  } else if ((index.data().toString() == "Отсрочено")) {
			return QVariant(QColor(90, 230, 100));
		  } else {
			return ind;
		  }
		}

	  default:
		return QVariant();
	}
  }
  return QVariant();
}
