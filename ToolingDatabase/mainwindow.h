#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QByteArray>
#include <QCloseEvent>
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include <QLine>
#include <QList>
#include <QMainWindow>
#include <QMarginsF>
#include <QMessageBox>
#include <QPagedPaintDevice>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QRegularExpression>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QTableView>
#include <QTextDocument>
#include <QTextStream>
#include <QWidget>
#include <QtPrintSupport>
#include <QtSql/QSqlDatabase>

#include "finddialog.h"
#include "myform.h"
#include "mysqltablemodel.h"

const QString PREFIX = "otpi_";
//const QString PATH_TO_DB = "D:/CPP/MyProects/DB_OTPI_SDI_4/TestDB.db";
const QString PATH_TO_DB = "/DB_OTPI.db"; //файл базы данных создастся сам!!!
const QString DRIVER_DB = "QSQLITE";
const QString MY_STYLE_CSS_FILE = ":/res/mystyle.css";

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  explicit MainWindow(QWidget *parent, const QString &fileName);
  ~MainWindow();

public:
  bool save();
  bool saveAs();

protected:
  void closeEvent(QCloseEvent *event);

private slots:
  void on_actionQuit_triggered();
  void on_actionSave_triggered();
  void on_actionOpen_triggered();
  void on_actionPasteRow_triggered();
  void on_actionDelRow_triggered();
  void on_actionNew_triggered();
  void on_actionSaveAs_triggered();
  void on_actionFind_triggered();
  void acceptorCloseFindDialogSlot();
  void acceptFindTextFromFindDialog(const QString &findText);
  bool on_actionConvertToCSV_triggered();
  void on_tableViewUi_pressed(const QModelIndex &index);
  void on_tableViewUi_doubleClicked(const QModelIndex &index);
  void on_actionPrinter_triggered();
  void printPreview(QPrinter *);
  void on_actionResetFind_triggered();

private:
  Ui::MainWindow *ui;
  bool connectToDb();
  void createViewTableAndSqlModel(const QString &tableName);
  void init();
  bool saveFile(const QString &fileNAme);
  void setCurrentFile(const QString &fileName);
  bool maybeSave();
  MainWindow * findMainWindow(const QString &fileName);
  void loadFile(const QString &fileName);
  void writeSettings();
  void readSettings();
  void setTableName(const QString &tableName);
  bool createNewTable(const QString &tableName);
  bool mySearchTableInDataBase(const QString &tableName);
  bool copyInSaveTableIsNoNameTable(const QString &tableName);
  bool copyInSaveTableIsTableToNewNameTable(const QString &newTableName);
  bool dropTableNoName()const;
  bool dropTable(const QString &tableName)const;
  void setTableAndFileNames(const QString &fileName, const QString tbName);
  void connections();
  void createSortProxyModel(const QRegularExpression &patternSearch);
  QRegularExpression mySetPattern(const QString &searchText);
  bool loadingStyleFileCss();

private:
  QString curFile; //путь к текущему файлу
  bool isUntitled; //флаг наличия имени и пути
  MySqlTableModel * mySqlTableModel;
  QSqlDatabase db;
  QString tableName;
  FindDialog * findDialog;
  bool findDialogIsShow;
  QSortFilterProxyModel * sortModel;
  MyForm *myEditForm;
};

#endif // MAINWINDOW_H
