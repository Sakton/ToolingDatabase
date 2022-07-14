#include "myform.h"
#include "ui_myform.h"

MyForm::MyForm(QWidget *parent) : QWidget(parent), ui(new Ui::MyForm)
{
  ui->setupUi(this);
  this->myMapWidget = new QDataWidgetMapper(this);
  this->setWindowTitle(tr("ПРАВКА"));
}

void MyForm::init()
{
  this->myMapWidget->addMapping(this->ui->lineEdit_vk, 1);
  this->myMapWidget->addMapping(this->ui->textEdit_Name, 2);
  this->myMapWidget->addMapping(this->ui->lineEdit_Izdely, 3);
  this->myMapWidget->addMapping(this->ui->lineEdit_Zakazchik, 4);
  this->myMapWidget->addMapping(this->ui->lineEdit_Phone, 5);
  this->myMapWidget->addMapping(this->ui->lineEdit_Card_Num, 6);
  this->myMapWidget->addMapping(this->ui->lineEdit_Zapis_Num, 7);
  this->myMapWidget->addMapping(this->ui->dateEdit_Start, 8);
  this->ui->dateEdit_Start->setDisplayFormat("dd.MM.yyyy");
  this->ui->dateEdit_Start->setDateTime(QDateTime::currentDateTime());
  this->myMapWidget->addMapping(this->ui->lineEdit_Zakaz_Num, 9);
  this->myMapWidget->addMapping(this->ui->dateEdit_End, 10);
  this->ui->dateEdit_End->setDisplayFormat("dd.MM.yyyy");
  this->ui->dateEdit_End->setDateTime(QDateTime::currentDateTime());
  this->myMapWidget->addMapping(this->ui->comboBox_Vypolneny, 11);
  this->myMapWidget->addMapping(this->ui->lineEdit_Status, 12);
  this->myMapWidget->addMapping(this->ui->textEdit_Prim, 13);
}

void MyForm::initForm(QSortFilterProxyModel *proxy)
{
  this->myMapWidget->setModel(proxy);
  this->init();
}

void MyForm::initForm(MySqlTableModel *myModel)
{
  this->myMapWidget->setModel(myModel);
  this->init();
}

void MyForm::setMyModel(MySqlTableModel *myModel)
{
  this->initForm(myModel);
  this->init();
}

void MyForm::setMyProxyModel(QSortFilterProxyModel *proxy)
{
  this->initForm(proxy);
  this->init();
}

QDataWidgetMapper *MyForm::getMyMapper()
{
  return this->myMapWidget;
}

void MyForm::on_pushButton_Save_clicked()
{
  this->myMapWidget->submit();
  this->close();
}

void MyForm::on_pushButton_Cancel_clicked()
{
  this->close();
}
