#include "mainwindow.h"

#include <QDebug>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setCurrentFile(QString());
  this->init();
}

MainWindow::MainWindow(QWidget *parent, const QString &fileName)
	: QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->loadFile(fileName);
  this->init();
}

MainWindow::~MainWindow() {
  delete ui;
}

bool MainWindow::save() {
  return isUntitled ? saveAs() : saveFile(this->curFile);
}

void MainWindow::init() {
  this->loadingStyleFileCss();
  this->isUntitled = true;
  this->readSettings();
  this->connectToDb();
  if (this->mySearchTableInDataBase("noName")) {
	this->dropTableNoName();
  }
  this->createNewTable(this->tableName);
  this->createViewTableAndSqlModel(this->tableName);
  this->connections();
  this->findDialogIsShow = false;
}

bool MainWindow::mySearchTableInDataBase(const QString &tableName) {
  QSqlQuery query;
  query.exec("select * from sqlite_master where type = 'table'");
  int i = 0;
  while (query.next()) {
	if (query.value(i).toString() == tableName) {
	  return true;
	}
	++i;
  }
  return false;
}

bool MainWindow::copyInSaveTableIsNoNameTable(const QString &tableName) {
  QSqlQuery *query = new QSqlQuery(this->db);
  bool res = query->exec("ALTER TABLE " + PREFIX + "noName RENAME TO " + tableName);
  if (!res) {
	return false;
  }
  return true;
}

bool MainWindow::copyInSaveTableIsTableToNewNameTable(const QString &newTableName) {
  if (this->createNewTable(newTableName)) {
	QSqlQuery *query = new QSqlQuery(this->db);
	bool res = query->exec("INSERT INTO " + newTableName + " SELECT * FROM "
						   + this->tableName);
	if (!res) {
	  this->dropTable(newTableName);
	  return false;
	}
  }
  return true;
}

bool MainWindow::dropTableNoName() const {
  QSqlQuery *query = new QSqlQuery(this->db);
  bool res = query->exec("DROP TABLE " + PREFIX + "noName");
  if (!res) {
	return false;
  }
  return true;
}

bool MainWindow::dropTable(const QString &tableName) const {
  QSqlQuery *query = new QSqlQuery(this->db);
  bool res = query->exec("DROP TABLE " + tableName);
  if (!res) {
	return false;
  }
  return true;
}

bool MainWindow::saveFile(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QFile::WriteOnly | QFile::Text)) {
	QMessageBox::warning(
		this, tr("Ошибка"),
		tr("не могу сохранить файл: %1:\n%2").arg(fileName, file.errorString()),
		QMessageBox::Ok);
	return false;
  }
  QString newNameTable = PREFIX + QFileInfo(fileName).baseName();
  QTextStream out(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  out << newNameTable;	//имя файла без расширения и пути
  QApplication::restoreOverrideCursor();
  file.close();
  this->setTableAndFileNames(fileName, newNameTable);
  this->ui->statusBar->showMessage(tr("Сохранено"));
  return true;
}

void MainWindow::setCurrentFile(const QString &fileName) {
  this->isUntitled = fileName.isEmpty();
  if (isUntitled) {
	this->curFile = tr("noName.otpi");
  } else {
	this->curFile = QFileInfo(fileName).canonicalFilePath();
  }
  this->tableName = PREFIX + QFileInfo(this->curFile).baseName();
  this->setWindowModified(
	  false);  //функция предопределена, утанавливает флаг был ли изменен документ
  this->setWindowTitle(this->tableName);
}

bool MainWindow::maybeSave() {
  QMessageBox *box = new QMessageBox(
	  QMessageBox::Warning, tr("Сохранение изменений"), tr("Сохранить изменения"),
	  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, this);

  // TODO кнопки
  //   box->addButton(QMessageBox::Save, tr("СОХРАНИТЬ"));
  //   box->addButton(QMessageBox::Discard, tr("НЕ СОХРАНЯТЬ"));
  //   box->addButton(QMessageBox::Cancel, tr("ОТМЕНА"));

  switch (box->exec()) {
	case QMessageBox::Save:
	  return this->save();
	case QMessageBox::Cancel:
	  return false;
	case QMessageBox::Discard:
	  this->close();
	  return true;
	default:
	  break;
  }
  return true;
}

MainWindow *MainWindow::findMainWindow(const QString &fileName) {
  QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
  foreach (QWidget *widget, QApplication::topLevelWidgets()) {
	MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
	if (mainWin && (mainWin->curFile == canonicalFilePath)) {
	  return mainWin;
	}
  }
  return nullptr;
}

void MainWindow::setTableAndFileNames(const QString &fileName, const QString tbName) {
  this->setTableName(tbName);
  this->createViewTableAndSqlModel(this->tableName);
  this->setCurrentFile(fileName);
  this->writeSettings();
}

void MainWindow::connections() {}

void MainWindow::createSortProxyModel(const QRegularExpression &patternSearch) {
  sortModel = new QSortFilterProxyModel(this);
  this->sortModel->setSourceModel(this->mySqlTableModel);
  this->ui->tableViewUi->setModel(this->sortModel);
  this->sortModel->setSortRole(Qt::DisplayRole);
  sortModel->setFilterRegularExpression(patternSearch);
  this->sortModel->setFilterKeyColumn(-1);
  this->ui->tableViewUi->resizeRowsToContents();
}

QRegularExpression MainWindow::mySetPattern(const QString &searchText) {
  return QRegularExpression(searchText);
}

bool MainWindow::loadingStyleFileCss() {
  QFile cssFile(MY_STYLE_CSS_FILE);
  if (!cssFile.open(QFile::ReadOnly)) {
	return false;
  }
  QString cssString = QLatin1String(cssFile.readAll());
  qApp->setStyleSheet(cssString);
  cssFile.close();
  return true;
}

void MainWindow::loadFile(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
	QMessageBox::warning(
		this, tr("Ошибка открытия файла"),
		tr("Не могу открыть файл %1/n%2").arg(fileName, file.errorString()));
	return;
  }
  QString tbName;
  QTextStream in(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  in >> tbName;
  QApplication::restoreOverrideCursor();
  file.close();
  this->setTableAndFileNames(fileName, tbName);
  this->ui->statusBar->showMessage("Загружаю", 2000);
}

void MainWindow::writeSettings() {
  QSettings settings("РКК", "ОТПИ");
  settings.setValue("pos", this->pos());  //установит текущее положение на экране
  settings.setValue("size", this->size());	//установит текуций размер
}

void MainWindow::readSettings() {
  QSettings settings("РКК", "ОТПИ");
  QPoint pos = settings.value("pos", QPoint(200, 200))
				   .toPoint();	//начальные настройки положения
  QSize size = settings.value("size", QSize(400, 400))
				   .toSize();  //начальные настройки размера окна
  this->move(pos);
  this->resize(size);
}

void MainWindow::setTableName(const QString &tableName) {
  this->tableName = tableName;
}

bool MainWindow::createNewTable(const QString &tableName) {
  QSqlQuery *query = new QSqlQuery(this->db);

  bool result = query->exec("CREATE TABLE " + tableName +
                            " ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "`vk` TEXT, `name` TEXT, `isdely` TEXT, "
                            "`zakazchik` TEXT, `phone` TEXT, `numCard` TEXT, "
                            "`slZapis` TEXT, `date` TEXT, "
                            "`numZakaz` TEXT, `srok` TEXT, `vypolnenye` TEXT, "
                            "`status` TEXT, `prim` TEXT )");
  if (!result) {
	return false;
  }
  return true;
}

bool MainWindow::saveAs() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить проект"),
												  this->curFile, tr("ОТПИ(*.otpi)"));
  if (fileName.isEmpty()) {
	return false;
  }
  QString newNameTable = PREFIX + QFileInfo(fileName).baseName();
  if (!this->mySearchTableInDataBase(newNameTable)) {
	if (isUntitled && (this->curFile == "noName.otpi")) {
	  if (this->copyInSaveTableIsNoNameTable(
			  newNameTable))  //если таблица удачно переименована с нонейм, сохраняем
							  //файл!!!!!!!!!!!!!
	  {
		this->saveFile(fileName);
		return true;
	  } else {
		QMessageBox::warning(this, tr("Ошибка"),
							 tr("не могу ПЕРЕИМЕНОВАТЬ таблицу: %1").arg(newNameTable),
							 QMessageBox::Ok);
		return false;
	  }
	} else if (!isUntitled && (this->tableName != newNameTable)) {
	  if (this->copyInSaveTableIsTableToNewNameTable(newNameTable)) {
		this->saveFile(fileName);
		return true;
	  } else {
		QMessageBox::warning(this, tr("Ошибка"),
							 tr("не могу СКОПИРОВАТЬ таблицу: %1").arg(newNameTable),
							 QMessageBox::Ok);
		return false;
	  }
	}
  } else {
	QMessageBox::warning(this, tr("Ошибка"),
						 tr("такая таблица есть в БД: %1").arg(fileName),
						 QMessageBox::Ok);
	return false;
  }
  return true;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (this->maybeSave()) {
	this->writeSettings();
	this->dropTableNoName();
	event->accept();
  } else {
	event->ignore();
  }
}

void MainWindow::on_actionQuit_triggered() {
  this->close();
}

void MainWindow::on_actionSave_triggered() {
  this->save();
}

void MainWindow::on_actionOpen_triggered() {
  QString fileName = QFileDialog::getOpenFileName(
	  this, tr("Открыть проект"), qApp->applicationDirPath(), tr("Проекты ОТПИ(*.otpi)"));
  if (!fileName.isEmpty()) {
	MainWindow *existing = this->findMainWindow(fileName);
	if (existing) {
	  existing->show();
	  return;
	}
  }
  if (isUntitled && !isWindowModified()) {
	this->loadFile(fileName);
  } else {
	MainWindow *other = new MainWindow(nullptr, fileName);
	other->setCurrentFile(fileName);
	other->move(x() + 40, y() + 40);
	other->show();
  }
}

bool MainWindow::connectToDb() {
  this->db = QSqlDatabase::addDatabase(DRIVER_DB);
  this->db.setDatabaseName(qApp->applicationDirPath() + PATH_TO_DB);
  if (!this->db.open()) {
	QMessageBox::warning(this, tr("ОШИБКА ПОДКЛЮЧЕНИЯ БД"),
						 tr("Нет подключения к базе данных."), QMessageBox::Ok);
	return false;
  } else {
	return true;
  }
}

void MainWindow::createViewTableAndSqlModel(const QString &tableName) {
  this->mySqlTableModel = new MySqlTableModel(this, this->db);
  this->mySqlTableModel->setTable(tableName);

  QList<QString> list;
  list << tr(" ID ") << tr(" ВК ") << tr(" НАИМЕНОВАНИЕ ") << tr(" ИЗДЕЛИЕ ")
	   << tr(" ЗАКАЗЧИК ") << tr(" ТЕЛЕФОН ") << tr(" № КАРТЫ ") << tr(" СЛ. ЗАПИСКА ")
	   << tr(" ДАТА ВЫДАЧИ") << tr(" № ЗАКАЗА ") << tr(" СРОК ") << tr(" ВЫПОЛНЕНИЕ ")
	   << tr(" СТАТУС ") << tr(" ПРИМЕЧАНИЕ ");
  for (int i = 0; i < list.size(); ++i) {
	this->mySqlTableModel->setHeaderData(i, Qt::Horizontal, list.value(i));
  }
  //Запрос
  this->mySqlTableModel->select();
  //стратегия редактирования
  this->mySqlTableModel->setEditStrategy(QSqlTableModel::OnRowChange);
  //включение кнопок:
  this->ui->actionDelRow->setEnabled(this->mySqlTableModel->rowCount() >= 1);
  this->ui->actionFind->setEnabled(this->mySqlTableModel->rowCount() >= 10);
  this->ui->actionConvertToCSV->setEnabled(this->mySqlTableModel->rowCount() >= 1);
  this->ui->actionPrinter->setEnabled(this->mySqlTableModel->rowCount() >= 1);
  this->ui->actionResetFind->setEnabled(this->mySqlTableModel->rowCount() >= 1);
  // cкрыть ID
  //Весь вывод через прокси модель !!!!!!
  this->createSortProxyModel(QRegularExpression());
  // this->ui->tableViewUi->setModel(mySqlTableModel);
  this->ui->tableViewUi->setModel(this->sortModel);
  // cкрыть ID в отображении!!!
  this->ui->tableViewUi->setColumnHidden(0, true);
  //размеры колонок
  QVector<int> sizeColumn;
  sizeColumn << 60 << 150 << 150 << 100 << 90 << 90 << 90 << 110 << 110 << 120 << 90
			 << 110 << 120 << 90;
  for (int i = 0; i < sizeColumn.size(); ++i) {
	this->ui->tableViewUi->setColumnWidth(i, sizeColumn.value(i));
  }
  //разрешение выделения ячеек
  this->ui->tableViewUi->setSelectionBehavior(QAbstractItemView::SelectRows);
  //Устанавливаем режим выделения лишь одно строки в таблице
  this->ui->tableViewUi->setSelectionMode(QAbstractItemView::SingleSelection);
  // Устанавливаем размер колонок по содержимому
  this->ui->tableViewUi->resizeRowsToContents();
}

void MainWindow::on_actionPasteRow_triggered() {
  int numRowSelected = this->ui->tableViewUi->currentIndex().row();
  if (numRowSelected == 0) {
	int row = this->mySqlTableModel->rowCount();
	this->mySqlTableModel->insertRow(row++);  //++row - добавит в начало списка
  } else {
	this->mySqlTableModel->insertRow(
		++numRowSelected);	// numRowSelected++ - добавит над выделенной строкой
  }
}

void MainWindow::on_actionDelRow_triggered() {
  QMessageBox *bx = new QMessageBox(QMessageBox::Warning, tr("УДАЛЕНИЕ ЗАПИСИ"),
									tr("Выделенная запись будет УДАЛЕНА!!!"),
									QMessageBox::Ok | QMessageBox::Cancel);
  bx->setButtonText(QMessageBox::Ok, tr("УДАЛИТЬ"));
  bx->setButtonText(QMessageBox::Cancel, tr("ОТМЕНИТЬ"));
  int ret = bx->exec();
  if (ret == QMessageBox::Ok) {
	if (this->ui->tableViewUi->currentIndex().isValid()) {
	  int numDelRow = this->ui->tableViewUi->currentIndex().row();
	  this->mySqlTableModel->removeRow(numDelRow);
	  this->mySqlTableModel->select();
	}
  } else {
	return;
  }
}

void MainWindow::on_actionNew_triggered() {
  MainWindow *other = new MainWindow();
  other->move(x() + 40, y() + 40);
  other->createNewTable(other->tableName);
  other->createViewTableAndSqlModel(other->tableName);
  other->show();
}

void MainWindow::on_actionSaveAs_triggered() {
  this->saveAs();
}

void MainWindow::on_actionFind_triggered() {
  if (!this->findDialogIsShow) {
	this->findDialog = new FindDialog(this);
	connect(this->findDialog, SIGNAL(sendFindTextFromFindDialog(QString)), this,
			SLOT(acceptFindTextFromFindDialog(QString)));
	connect(this->findDialog, SIGNAL(closeFindDialogSignal()), this,
			SLOT(acceptorCloseFindDialogSlot()));
	this->findDialog->show();
	this->findDialogIsShow = true;
  } else {
	this->findDialogIsShow = false;
	return;
  }
}

void MainWindow::acceptorCloseFindDialogSlot() {
  this->findDialogIsShow = false;
}

void MainWindow::acceptFindTextFromFindDialog(const QString &findText) {
  this->createSortProxyModel(this->mySetPattern(findText));
}

bool MainWindow::on_actionConvertToCSV_triggered() {
  if (this->mySqlTableModel->rowCount()) {
	QString csvFileName = QFileDialog::getSaveFileName(this, tr("CSV FILE CONVERT"),
													   tr(""), tr("ЭКСЕЛЬ(*.csv)"));
	if (csvFileName.isEmpty()) {
	  QMessageBox::warning(this, tr("НЕТ ИМЕНИ ФАЙЛА"),
						   tr("Надо задать имя файла для сохранения"),
						   QMessageBox::Ok | QMessageBox::Cancel);
	  return false;
	}
	QFile csvFile(csvFileName);
	if (!csvFile.open(QFile::WriteOnly | QFile::Text)) {
	  QMessageBox::warning(
		  this, tr("ОШИБКА ОТКРЫТИЯ ФАЙЛА"),
		  tr("ОШИБКА ОТКРЫТИЯ ФАЙЛА ДЛЯ ЗАПИСИ, ИЛИ ЭТОТ ФАЙЛ ОТКРЫТ В ДРУГОЙ ПРОГРАММЕ"),
		  QMessageBox::Ok);
	  return false;
	}
	QStringList strList;
	QTextStream ts(&csvFile);
	strList << "\"\"";
	for (int i = 0; i < this->ui->tableViewUi->horizontalHeader()->count(); ++i) {
	  strList << "\""
					 + this->ui->tableViewUi->model()
						   ->headerData(i, Qt::Horizontal)
						   .toString()
					 + "\"";
	}
	ts << strList.join(";") + "\n";
	for (int j = 0; j < this->ui->tableViewUi->verticalHeader()->count(); ++j) {
	  strList.clear();
	  strList
		  << "\"" + this->ui->tableViewUi->model()->headerData(j, Qt::Vertical).toString()
				 + "\"";
	  for (int k = 0; k < this->ui->tableViewUi->horizontalHeader()->count(); ++k) {
		strList << "\""
					   + this->ui->tableViewUi->model()
							 ->data(this->ui->tableViewUi->model()->index(j, k),
									Qt::DisplayRole)
							 .toString()
					   + "\"";
	  }
	  ts << strList.join(";") + "\n";
	}
	csvFile.close();
	return true;
  }
  return false;
}

void MainWindow::on_tableViewUi_pressed(
	const QModelIndex &index)  //вспомогательная функция, убрать на релиз-версии
{
  if (index.isValid()) {
  }	 //чушь от желтых треугольников
}

void MainWindow::on_tableViewUi_doubleClicked(const QModelIndex &index) {
  this->myEditForm = new MyForm();
  this->myEditForm->setParent(this, Qt::Window);
  this->myEditForm->setWindowModality(Qt::WindowModal);
  this->myEditForm->setMyProxyModel(this->sortModel);
  this->myEditForm->getMyMapper()->setCurrentModelIndex(index);
  this->myEditForm->show();
}

void MainWindow::on_actionPrinter_triggered() {
  // TODO переписать
  //  QPrinter *myPrinter = new QPrinter(QPrinter::PrinterResolution);
  //  myPrinter->setPaperSize(QPrinter::A4);
  //  myPrinter->setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
  //  myPrinter->setPageOrientation(QPageLayout::Landscape);
  //  QPrintPreviewDialog *prew = new QPrintPreviewDialog(myPrinter, this);
  //  prew->setWindowTitle(tr("ПРЕДПРОСМОТР"));
  //  prew->setMinimumSize(QSize(1000, 700));

  //  connect(prew, SIGNAL(paintRequested(QPrinter *)), this, SLOT(printPreview(QPrinter
  //  *))); prew->exec();
}

void MainWindow::printPreview(QPrinter *myPrinter) {
  QAbstractItemModel *model = this->ui->tableViewUi->model();
  qDebug() << "Пустая модель? " << model;

  //**********************************************************************
  //ВЕРСИЯ ЧЕРЕЗ HTML
  if (model) {
	QHeaderView *hHeader = this->ui->tableViewUi->horizontalHeader();
	QString myHtml = "";
	QTextDocument *doc = new QTextDocument();
	//установить размеры страницы документа!!!
	doc->setPageSize(QSizeF(myPrinter->width() / 6, myPrinter->height() / 6));
	doc->setDocumentMargin(0);
	int columnCount = hHeader->count();
	int rowCount = model->rowCount(this->ui->tableViewUi->rootIndex());

	myHtml += tr("<html><body><table cellpadding=2 border=1>");
	if (doc->pageCount() == 1) {
	  myHtml += tr(
		  "<tr style=\'font:15px; font-style:italic; "
		  "background-color:rgb(200,200,200);\';><th colspan=13>");
	  myHtml += tr("Оснастка, учтенная отделом ОТПИ КИС-416Ц, по состоянию на: ");
	  myHtml += QDate::currentDate().toString("dd.MM.yyyy");
	  myHtml += tr("</th></tr>");
	  // myPrinter->setPageOrder(QPrinter::LastPageFirst); //????

	  myHtml += tr("<tr>");
	  for (int i = 0; i < columnCount; ++i) {
		if (!hHeader->isSectionHidden(i)) {
		  if (i != 13) {
			myHtml += tr(
				"<td align=\'center\' valign=\'middle\' style=\' font:10px; "
				"font-weight:bold;"
				"background-color:rgb(180,180,180);\'>");
		  } else {
			myHtml += tr(
				"<td align=\"center\" valign=\"middle\" width=150 style=\' font:10px; "
				"font-weight:bold;"
				"background-color:rgb(180,180,180);\'>");
		  }
		  myHtml += model->headerData(i, Qt::Horizontal).toString();
		  myHtml += tr("</td>");
		}
	  }
	}
	myHtml += tr("</tr>");

	for (int pg = 0; pg < doc->pageCount(); ++pg) {
	  for (int j = 0; j < rowCount; ++j) {
		myHtml += tr("<tr>");
		for (int i = 0; i < columnCount; ++i) {
		  if (!hHeader->isSectionHidden(i)) {
			myHtml += tr("<td align=\"center\" valign=\"middle\">");
			myHtml += model->data(model->index(j, i)).toString();
			myHtml += tr("</td>");
		  }
		}
		myHtml += tr("</tr>");
	  }
	  if (pg + 1 < doc->pageCount()) {
		myHtml += "<br style=\'page-break-after:always\'>";
	  }
	}
	myHtml += tr("</table></body></html>");
	doc->setHtml(myHtml);
	doc->print(myPrinter);
	delete doc;

	qDebug() << "qApp->applicationDirPath()" << qApp->applicationDirPath();
	qDebug() << "qApp->applicationFilePath()" << qApp->applicationFilePath();
  }
  //**********************************************************************
  //ВЕРСИЯ ПО ДОКУМЕННТАЦИИ
  //    if(model)
  //    {
  //        QTextDocument *doc = new QTextDocument();
  //        //настраиваем документ
  //        doc->setPageSize(QSizeF(myPrinter->width()/6,myPrinter->height()/6));
  //        doc->setDocumentMargin(0);
  //        //главный фрейм
  //        QTextFrame *topFrame = doc->rootFrame();
  //        QTextFrameFormat topFrameFormat = topFrame->frameFormat();
  //        topFrameFormat.setBorder(1);
  //        topFrameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  //        topFrameFormat.setBorderBrush(QBrush(Qt::black));

  //        topFrame->setFrameFormat(topFrameFormat);

  //        //делаю текстовый курсор
  //        //QTextCursor cursor(doc);
  //        QTextCursor cursor(topFrame);
  //        //настройка блока текста для вставки
  //        QTextBlockFormat blockFormat;
  //        blockFormat.setBackground(QBrush(QColor(200,200,200)));
  //        blockFormat.setAlignment(Qt::AlignCenter);
  //        cursor.setBlockFormat(blockFormat);
  //        //настройка шрифта для блока

  //        QTextCharFormat textFormatForTopBlock;
  //        //выбор шрифта и его настройка
  //        QFont myFont("Helvetica");
  //        myFont.setPointSizeF(12);
  //        myFont.setBold(true);
  //        myFont.setItalic(true);
  //        textFormatForTopBlock.setFont(myFont);

  //        QTextCharFormat textFormatForHeader;
  //        QFont myFontHeader("Helvetica");
  //        myFontHeader.setPointSizeF(8);
  //        myFontHeader.setBold(true);
  //        textFormatForHeader.setFont(myFontHeader);

  //        QString nameTable = tr("Оснастка, учтенная отделом ОТПИ КИС-416Ц, по состоянию
  //        на: "); nameTable += QDate::currentDate().toString("dd.MM.yyyy");
  //        cursor.insertText(nameTable, textFormatForTopBlock);

  //        QTextTableFormat myTableFormat;
  //        myTableFormat.setCellPadding(2);
  //        myTableFormat.setCellSpacing(0);
  //        //myTableFormat.setAlignment(Qt::AlignCenter/* | Qt::AlignVertical_Mask*/);
  //        //myTableFormat.setHeaderRowCount(1);

  //        QTextBlockFormat cellBlockFormat;
  //        cellBlockFormat.setAlignment(Qt::AlignCenter);
  //        cellBlockFormat.setBackground(QBrush(Qt::yellow));

  //        QTextBlockFormat cellFormat;

  //        cellFormat.setAlignment(Qt::AlignCenter);

  //        int row = model->rowCount(this->ui->tableViewUi->rootIndex());
  //        int column = model->columnCount();

  //        QTextTable *table = cursor.insertTable(row+1 ,column-1, myTableFormat);
  //        //i-1 смещение для непоказа колонки с ID

  //        qDebug() << "cellPadding() " << myTableFormat.cellPadding();
  //        qDebug() << "cellSpacing() " << myTableFormat.cellSpacing();

  //            for(int i = 1; i < column; ++i)
  //            {
  //                cursor = table->cellAt(0, i-1).firstCursorPosition(); //Возвращает
  //                первую действительную позицию курсора в этой клетке.
  //                cursor.insertBlock(cellBlockFormat, textFormatForHeader);
  //                cursor.insertText(model->headerData(i, Qt::Horizontal).toString());
  //            }
  //            //j+1 смещение для пропуска первой заполненной строки под заголовки
  //            for(int j = 0; j < row; ++j)
  //              {
  //                for(int i = 1; i < column; ++i)
  //                  {
  //                    if(table->cellAt(j+1, i-1).isValid())
  //                      {
  //                        cursor = table->cellAt(j+1, i-1).firstCursorPosition();
  //                        cursor.insertBlock(cellFormat);
  //                        cursor.insertText(model->data(this->ui->tableViewUi->model()->index(j,i)).toString());
  //                      }
  //                  }
  ////                for(int pg = 0; pg < doc->pageCount(); ++pg)
  ////                  {
  ////                    if(pg+1<doc->pageCount())
  ////                      {
  ////                        QTextBlockFormat blockFormat;
  //// blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysAfter); /
  /////cursor.mergeBlockFormat(blockFormat); /                      } /                  }
  //              }
  //            qDebug() << "qApp->applicationDirPath()" << qApp->applicationDirPath();
  //            qDebug() << "qApp->applicationFilePath()" << qApp->applicationFilePath();
  //        doc->print(myPrinter);
  //        delete doc;
  //    }
}

void MainWindow::on_actionResetFind_triggered() {
  this->createViewTableAndSqlModel(this->tableName);
}
