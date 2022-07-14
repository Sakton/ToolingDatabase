//*********************

//void MainWindow::writeSettings()
//{
//    QSettings settings(/*QCoreApplication::organizationName()*/"РКК", /*QCoreApplication::applicationName()*/"OTPI");
//    settings.setValue("geometry", this->saveGeometry());
//    settings.setValue("organisation", "РКК");
//    settings.setValue("application", "OTPI_DB");
//}

//??????????????????????????????????????????????????????????????
//??????????????????????????????????????????????????????????????

//void MainWindow::readSettings() //разобрать функцию
//{
//    QSettings settings(/*QCoreApplication::organizationName()*/"РКК", /*QCoreApplication::applicationName()*/"OTPI");
//    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
//    if(geometry.isEmpty()){
//        //const QRect avialableGeometry = QDesktopWidget::availableGeometry(this);
//        const QRect avialableGeometry = QApplication::desktop()->availableGeometry(this);
//        this->resize(avialableGeometry.width()/3, avialableGeometry.height()/2);
//        this->move((avialableGeometry.width() - this->width())/2, (avialableGeometry.height() - this->height())/2);
//    }else{
//        this->restoreGeometry(geometry);
//    }
//}

//*******************

//??????????????????????????????????????????????????????????????
//??????????????????????????????????????????????????????????????

//void MainWindow::prependToRecentFiles(const QString &fileName)
//{
//    QSettings settings(/*QCoreApplication::organizationName()*/"РКК", /*QCoreApplication::applicationName()*/"OTPI");
//    const QStringList oldRecentFiles = this->readRecentFiles(settings);
//    QStringList recentFiles = oldRecentFiles;
//    recentFiles.removeAll(fileName);
//    recentFiles.prepend(fileName);
//    if(oldRecentFiles != recentFiles){
//        this->writeRecentFiles(recentFiles, settings);
//    }
//    setRecentFileVisible(!recentFiles.isEmpty());//?????видимость меню
//}

//??????????????????????????????????????????????????????????????
//??????????????????????????????????????????????????????????????

//QStringList MainWindow::readRecentFiles(QSettings &settings)
//{
//    QStringList result;
//    const int count = settings.beginReadArray(this->recentFilesKey());
//    for(int i = 0; i < count; ++i){
//        settings.setArrayIndex(i);
//        result.append(settings.value(this->fileKey()).toString());
//        qDebug() << settings.value(this->fileKey()).toString();
//    }
//    settings.endArray();
//    return result;
//}

//??????????????????????????????????????????????????????????????
//??????????????????????????????????????????????????????????????

//void MainWindow::setRecentFileVisible(bool visible)
//{
//    //что то с видимостью меню, разобраться...
//    visible = true;
//    //return visible = true;
//}

//??????????????????????????????????????????????????????????????
//??????????????????????????????????????????????????????????????

//inline QString MainWindow::recentFilesKey(){//?????????????????????????????????????????
//    return QStringLiteral("recentFileList"); //???????????????????????????????????
//}

//??????????????????????????????????????????????????????????????
//??????????????????????????????????????????????????????????????

//inline QString MainWindow::fileKey(){
//    return QStringLiteral("file");
//}

//??????????????????????????????????????????????????????????????
//??????????????????????????????????????????????????????????????

//void MainWindow::writeRecentFiles(const QStringList &files, QSettings &settings){
//    const int count = files.size();
//    settings.beginWriteArray(this->recentFilesKey());
//    for(int i = 0; i < count; ++i){
//        settings.setArrayIndex(i);
//        settings.setValue(this->fileKey(), files.at(i));
//    }
//    settings.endArray();
//}

//*****************

//bool MainWindow::saveFile(const QString &fileName)
//{
//    QFile file(fileName);
//    if(!file.open(QFile::WriteOnly | QFile::Text)){
//        QMessageBox::warning(this, tr("Ошибка"), tr("не могу сохранить файл: %1:\%2").arg(QDir::toNativeSeparators(fileName), file.errorString()), QMessageBox::Ok);
//        return false;
//    }
//    QString newNameTable = PREFIX + QFileInfo(fileName).baseName();
//    if(newNameTable != "noName" && this->isUntitled){
//        if(!this->mySearchTableInDataBase(newNameTable)){
//                qDebug() << "Первый if this->mySearchTableInDataBase(newNameTable)" << newNameTable;
//                if(this->copyInSaveTableIsNoNameTable(newNameTable)){ //если таблица удачно переименована с нонейм, сохраняем файл!!!!!!!!!!!!!
//                    qDebug() << "Второй if copyInSaveTableIsNoNameTable(newNameTable)";
//                    QTextStream out(&file);
//                    QApplication::setOverrideCursor(Qt::WaitCursor);
//                    out << newNameTable; //имя файла без расширения и пути
//                    QApplication::restoreOverrideCursor();
//                    file.close();
//                }else{
//                    QMessageBox::warning(this, tr("Ошибка"), tr("не могу переименовать в таблицу: %1").arg(newNameTable), QMessageBox::Ok);
//                    return false;
//                }
//        }else{
//            /*QMessageBox::StandardButton ret =*/ QMessageBox::warning(this, tr("Ошибка"), tr("такой файл уже есть: %1").arg(fileName), QMessageBox::Ok);
////            if(ret == QMessageBox::Ok){
////                this->save();
////            }
//        }
//    }else{
//        /*QMessageBox::StandardButton ret =*/ QMessageBox::warning(this, tr("Ошибка"), tr("Надо дать имя проекту"), QMessageBox::Ok);
//                //ДОДЕЛАТЬ!!
////        if(ret == QMessageBox::Ok){
////            this->save();
////        }
//    }
//    this->setCurrentFile(fileName);
//    this->createViewTableAndSqlModel(newNameTable);
//    qDebug() << "saveFile" << this->tableName;
//    this->writeSettings();
//    this->ui->statusBar->showMessage(tr("Сохранено"));
//    //this->dropTableNoName();
//    return true;
//}

//*************************************
//???????????????????????????
//QSqlQuery query;
//query.exec("SELECT COUNT(*) FROM" + tableName);
//qDebug() << query.exec("SELECT COUNT(*) FROM" + tableName);
//qDebug() << query.value("COUNT(*)").toInt();

//?????????????
//QSqlQuery *query = new QSqlQuery(db);
//query->exec("SELECT COUNT(*) FROM " + tableName);
//while(query->next()){
//    qDebug() << query->value("COUNT(*)").toInt;
//}
//qDebug() << query->exec("SELECT * FROM " + tableName);
//int x = query->size();
//qDebug() << x;

//ПЕЧАТЬ ПРИНТЕР ЧАСТЬ КОДА

//        if(model)
//        {
//            QPainter painter(myPrinter);
//            if(!painter.isActive())
//            {
//                return;
//            }
//            int pageLeft = 0;
//            int pageTop = 0;
//            int pageHeight = painter.device()->height();
//            int pageWidth = painter.device()->width();
//            int x = pageLeft;
//            int y = pageTop;

//            QList<QVector<QLine> > linePages; //Список<Вектор<класс-линия> >
//            QVector<QLine> verticalLines;     //Вектор<класс-линия>
//            QList<QList<int> > sectionPages;  //Список<Список<тип ИНТ> >
//            QList<int> sections;              //Список<тип ИНТ>
//            QList<QList<int> > widthPages;    //Список<Список<тип ИНТ> >
//            QList<int> widths;                //Список<тип ИНТ>
//            QList<int> pageRights;            //Список<тип ИНТ>
//            QList<QStringList> pageLabels;    //Список<тип СТРИНГ>
//            QStringList labels;
//            int logicalIndex;
//            //столбцы
//            for(int i = 0; i < hHeader->count(); ++i)
//            {
//                logicalIndex = hHeader->logicalIndex(i);
//                //если столбец не скрыт
//                if(!hHeader->isSectionHidden(logicalIndex))
//                {
//                    //если нет места на странице пошлем на новую страницу ??
//                    if(pageWidth <= x + hHeader->sectionSize(logicalIndex))
//                    {
//                        verticalLines.append(QLine(x, pageTop, x, pageHeight));
//                        pageRights.append(x);
//                        x = pageLeft;
//                        sectionPages.append(sections);
//                        sections.clear();
//                        linePages.append(verticalLines);
//                        verticalLines.clear();
//                        widthPages.append(widths);
//                        widths.clear();
//                        pageLabels.append(labels);
//                        labels.clear();
//                    }
//                    //добавляем, так как колонка видимая
//                    sections.append(logicalIndex);
//                    widths.append(hHeader->sectionSize(logicalIndex));
//                    labels.append(model->headerData(logicalIndex, Qt::Horizontal).toString());
//                    verticalLines.append(QLine(x, pageTop, x, pageHeight));
//                    //смещение на ширину колонки
//                    x += hHeader->sectionSize(logicalIndex);
//                }
//            }

//            if(!sections.isEmpty())
//            {
//                verticalLines.append(QLine(x, pageTop, x, pageHeight));
//                pageRights.append(x);
//                x = pageLeft;
//                sectionPages.append(sections);
//                sections.clear();
//                linePages.append(verticalLines);
//                verticalLines.clear();
//                widthPages.append(widths);
//                widths.clear();
//                pageLabels.append(labels);
//                labels.clear();
//            }

//            int columnPageCount = sectionPages.count();
//            int columnCount = sections.count();
//            int rowCount = model->rowCount(this->ui->tableViewUi->rootIndex()); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//            int rowHeight;
//            int columnWidth;
//            QModelIndex currentIndex;
//            int row = 0;
//            int lastPageRow = 0;
//            int columnPageIndex = 0;
//            int rightColumn;

//            while(true)
//            {
//                //колонки текущей страницы
//                sections = sectionPages.at(columnPageIndex);
//                widths = widthPages.at(columnPageIndex);
//                verticalLines = linePages.at(columnPageIndex);
//                //количество колонок текущей страницы
//                columnCount = sections.count();
//                rightColumn = pageRights.at(columnPageIndex);
//                labels = pageLabels.at(columnPageIndex);
//                //перемещаемся в левый верхний угол страницы
//                x = pageLeft;
//                y = pageTop;
//                rowHeight = hHeader->height();
//                //рисование колонок
//                for(int i = 0; i < columnCount; ++i)
//                {
//                    //текущее значение
//                    columnWidth = widths.at(i);
//                    painter.drawText(x, y, columnWidth, rowHeight, currentIndex.data(Qt::TextAlignmentRole).toInt()//????
//                                     | Qt::TextWordWrap
//                                     | Qt::AlignVCenter
//                                     | Qt::AlignHCenter,
//                                     labels.at(i)
//                                     );
//                    painter.drawLine(pageLeft, y, rightColumn, y);
//                    //Перемещение правее
//                    x += widths.at(i);
//                }
//                y += rowHeight;
//                x = pageLeft;
//                //пока можем рисовать на текущей странице
//                while (true)
//                {
//                    // Высота текущей строки
//                    rowHeight = vHeader->sectionSize(row);
//                    if (y + rowHeight > pageHeight)
//                    {
//                        //--row;
//                        break;
//                    }
//                    // Рисуем колонки
//                    for (int i = 0; i < columnCount; ++i)
//                    {
//                        // Текущее значение
//                        currentIndex = model->index(row, sections.at(i), this->ui->tableViewUi->rootIndex());
//                        columnWidth = widths.at(i);

//                        if (!currentIndex.data().isNull())
//                        {
//                            painter.drawText(x, y, columnWidth, rowHeight
//                                             , currentIndex.data(Qt::TextAlignmentRole).toInt()
//                                             | Qt::TextWordWrap
//                                             | Qt::AlignVCenter
//                                             | Qt::AlignHCenter
//                                             , currentIndex.data().toString()
//                                             );
//                        }
//                        painter.drawLine(pageLeft, y, rightColumn, y);
//                        // Перемащаемся правее
//                        x += widths.at(i);
//                    }
//                    x = pageLeft;
//                    // Перемещаемся ниже
//                    y += rowHeight;
//                    // Берем следующую строку
//                    ++row;
//                    // Если строки кончились
//                    if (row >= rowCount)
//                    {
//                        break;
//                    }
//                }
//                // Рисуем завершающую горизонтальную линию
//                painter.drawLine(pageLeft, y, rightColumn, y);
//                // Рисуем вертикальные линии
//                painter.drawLines(verticalLines);
//                // Вытираем лишние концы
//                // TODO: hard-coded background color for page
//                painter.fillRect(pageLeft, y + 1, pageWidth, pageHeight - (y + 1), QBrush(Qt::white));

//                // Если предыдущие колонки не поместились
//                if (columnPageIndex < columnPageCount - 1)
//                {
//                    ++columnPageIndex;
//                    row = lastPageRow;
//                    // Предыдущие колонки поместились
//                }
//                else
//                {
//                    columnPageIndex = 0;
//                    lastPageRow = row;

//                    // Строки закончились
//                    if (row >= rowCount)
//                    {
//                        break;
//                    }
//                }

//                // Создаем новую страницу
//                myPrinter->newPage();
//                }
//            }



////            QTextBrowser *preview = new QTextBrowser();
////            preview->setMinimumWidth(myPrinter->width());
////            preview->setMinimumHeight(myPrinter->height());
////            preview->setHtml(myHtml);
////            preview->show();
