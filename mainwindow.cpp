#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deldialog.h"
#include "ipdialog.h"
#include <QErrorMessage>
#include <QDateTime>
#include <QMessageBox>

MainWindow::MainWindow(DbWorker dbConnect, QWidget *parent) :
    dbConnect(dbConnect),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QPoint pos( X_POSITION_FRAME, Y_POSITION_FRAME );
    this->move( pos );
    ui->setupUi( this );
    setWindowTitle("Объект управления");
    makeLogNote("Начало работы");
    udpSocket.bind( LISTERNING_PORT );
    converter = new Converter();
    //setTargetIp();
    //setMyIp();
    myIp.setAddress("192.168.1.87");
    targetIp.setAddress("192.168.1.180");
    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(readDatagram()));
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    QDateTime time(QDate(2017, 3, 30), QTime(13, 43, 48));
    //1499771506168
    on_updBut_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete converter;
}

void MainWindow::makeLogNote(QString s)
{
    ui->logField->append( tr( "%1 %2 %3" ).arg( QDate::currentDate().toString( "dd.MM.yyyy" ) )
                                          .arg( QTime::currentTime().toString( "hh:mm:ss" ) )
                                          .arg( s ) );
}

void MainWindow::on_exitButton_clicked()
{
   this->close();
}

void MainWindow::on_updBut_clicked()
{
    QSqlQuery query = QSqlQuery(dbConnect.getDb());
    //подумать что делать с пунктом время исполнения
    QString selectPattern = "SELECT  inf.order_id, t1.termname, inf.date_add "
                "FROM orders_alerts.orders_alerts_info  inf "
                "JOIN reference_data.terms t1 ON inf.order_tid = t1.termhierarchy;";
    if (!query.exec(selectPattern)) {
        qDebug() << "Unable to make select operation!" << query.lastError();
    }
    ui->commandTable->setRowCount(query.size());
    int i= 0;
    while (query.next()) {
        ui->commandTable->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->commandTable->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->commandTable->setItem(i, 2, new QTableWidgetItem(query.value(2).toDateTime().toString("dd.MM.yyyy hh:mm:ss")));
        i++;
    }
    selectPattern = "SELECT cinf.outgoing_reg_number, cinf.outgoing_reg_datetime, t1.termname, t2.termname, cinf.holder_coid "
            "FROM combatdocs.combatdocs_info cinf "
              "JOIN combatdocs.combatdocs_type ctyp ON cinf.cmbdid = ctyp.cmbdid "
              "JOIN combatdocs.combatdocs_theme cthm ON cinf.cmbdid = cthm.cmbdid "
              "JOIN reference_data.terms t1 ON ctyp.doctype_tid = t1.termhierarchy "
              "JOIN reference_data.terms t2 ON cthm.doctheme_tid = t2.termhierarchy;";
    if (!query.exec(selectPattern)) {
        qDebug() << "Unable to make select operation!" << query.lastError();
    }
    ui->documentTable->setRowCount(query.size());
    i = 0;
    while (query.next()) {
        ui->documentTable->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->documentTable->setItem(i, 1, new QTableWidgetItem(query.value(1).toDateTime().toString("dd.MM.yyyy hh:mm:ss")));
        ui->documentTable->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->documentTable->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->documentTable->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
        i++;
    }
    selectPattern = "SELECT combat_hierarchy,"
            "ST_X(obj_location), ST_Y(obj_location), ST_Z(obj_location), direction,"
              "date_add, date_edit, date_delete, id_manager FROM own_forces.combatobject_location;";
    if (!query.exec(selectPattern)) {
        qDebug() << "Unable to make select operation!" << query.lastError();
    }
    ui->coordinatTable->setRowCount(query.size());
    i = 0;
    while (query.next()) {
        QString coordinats = "" + query.value(1).toString() + " N " + query.value(2).toString() + " E " + query.value(3).toString();
        ui->coordinatTable->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->coordinatTable->setItem(i, 1, new QTableWidgetItem(coordinats));
        ui->coordinatTable->setItem(i, 2, new QTableWidgetItem(query.value(4).toString()));
        ui->coordinatTable->setItem(i, 3, new QTableWidgetItem(query.value(5).toDateTime().toString("dd.MM.yyyy hh:mm:ss")));
        ui->coordinatTable->setItem(i, 4, new QTableWidgetItem(query.value(6).toDateTime().toString("dd.MM.yyyy hh:mm:ss")));
        ui->coordinatTable->setItem(i, 5, new QTableWidgetItem(query.value(7).toDateTime().toString("dd.MM.yyyy hh:mm:ss")));
        ui->coordinatTable->setItem(i, 6, new QTableWidgetItem(query.value(8).toString()));
        i++;
    }
    selectPattern = "SELECT combat_hierarchy, currentmode_tid, date_add, date_edit, date_delete, id_manager FROM own_forces.currentmode;";
    if (!query.exec(selectPattern)) {
        qDebug() << "Unable to make select operation!" << query.lastError();
    }
    ui->modeTable->setRowCount(query.size());
    i = 0;
    while (query.next()) {
        ui->modeTable->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->modeTable->setItem(i, 1, new QTableWidgetItem(Utility::convertCodeToReferenceName(dbConnect.getDb(), query.value(1).toString())));
        ui->modeTable->setItem(i, 2, new QTableWidgetItem(query.value(2).toDateTime().toString("dd.MM.yyyy hh:mm:ss")));
        ui->modeTable->setItem(i, 3, new QTableWidgetItem(query.value(3).toDateTime().toString("dd.MM.yyyy hh:mm:ss")));
        ui->modeTable->setItem(i, 4, new QTableWidgetItem(query.value(4).toDateTime().toString("dd.MM.yyyy hh:mm:ss")));
        ui->modeTable->setItem(i, 5, new QTableWidgetItem(query.value(5).toString()));
        i++;
    }
    selectPattern = "SELECT id_note, type_message, data, receiver_sender, \"package\", status FROM log.log_table_message;";
    if (!query.exec(selectPattern)) {
        qDebug() << "Unable to make select operation!" << query.lastError();
    }
    ui->logTable->setRowCount(query.size());
    i = 0;
    while (query.next()) {
        ui->logTable->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->logTable->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        ui->logTable->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
        ui->logTable->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
        ui->logTable->setItem(i, 4, new QTableWidgetItem(query.value(4).toString()));
        ui->logTable->setItem(i, 5, new QTableWidgetItem(query.value(5).toString()));
        i++;
    }
    makeLogNote("данные обновлены");
    for ( int i = 0; i < ui->documentTable->columnCount(); i++ ) {
        ui->documentTable->horizontalHeader()->setSectionResizeMode( i , QHeaderView::ResizeToContents);
    }
    for ( int i = 0; i < ui->commandTable->columnCount(); i++ ) {
        ui->commandTable->horizontalHeader()->setSectionResizeMode( i , QHeaderView::ResizeToContents);
    }
    for ( int i = 0; i < ui->coordinatTable->columnCount(); i++ ) {
        ui->coordinatTable->horizontalHeader()->setSectionResizeMode( i , QHeaderView::ResizeToContents);
    }
    for ( int i = 0; i < ui->modeTable->columnCount(); i++ ) {
        ui->modeTable->horizontalHeader()->setSectionResizeMode( i , QHeaderView::ResizeToContents);
    }
}

void MainWindow::on_clearBut_clicked()
{
    ui->logField->clear();
}

void MainWindow::on_itemSendCoord_triggered()
{
    QString trash;
    DelDialog dia;
    if ( dia.exec() ) {
        trash = dia.value();
    }
    QString data = makeDatagramCoord( trash );
    if ( data == "error" ) {
        makeLogNote( "ошибка создания датаграммы" );
        QMessageBox::information(this, "ОШИБКА", "такой записи не существует!");
        return;
    }
    QStringList list;
    list << myIp.toString()
         << targetIp.toString()
         << "0"
         << "17"
         << QString::number( data.length() + 224 )
         << myPort
         << targetPort
         << QString::number( data.length() )
         << "14"
         << "0001"
         << QString::number( unicumMessageId )
         << "1"
         << "1"
         << data;
    unicumMessageId++;
    QByteArray datagram = converter->encodeDatagram( list );
    qDebug() << targetPort.toLong( Q_NULLPTR, 10 );
    udpSocket.writeDatagram( datagram, targetIp, targetPort.toLong( Q_NULLPTR, 10) );
    makeLogNote( "отправлен пакет" );
    QMessageBox::information(this, "УСПЕХ", "Пакет отправлен успешно");
    bool x = dbConnect.makeNote( 1, getCurrentDateAndTime(), 1, data, 2);
    if ( x ) {
        makeLogNote( "запись действия добавлена в БД" );
    }
        else {
        makeLogNote( "ошиба записи действия в БД" );
    }
}

void MainWindow::on_itemSendRocket_triggered()
{
    QString trash;
    DelDialog dia;
    if ( dia.exec() ) {
        trash = dia.value();
    }
    QString data = makeDatagramRocket( trash );
    if ( data == "error" ) {
        makeLogNote( "ошибка создания датаграммы" );
        QMessageBox::information(this, "ОШИБКА", "такой записи не существует!");
        return;
    }
    QStringList list;
    list << myIp.toString()
         << targetIp.toString()
         << "0"
         << "17"
         << QString::number( data.length() + 224 )
         << myPort
         << targetPort
         << QString::number( data.length() )
         << "14"
         << "0001"
         << QString::number( unicumMessageId )
         << "1"
         << "1"
         << data;
    unicumMessageId++;
    QByteArray datagram = converter->encodeDatagram( list );
    qDebug() << targetPort.toLong( Q_NULLPTR, 10 );
    udpSocket.writeDatagram( datagram, targetIp, targetPort.toLong( Q_NULLPTR, 10) );
    makeLogNote( "отправлен пакет" );
    QMessageBox::information(this, "УСПЕХ", "Пакет отправлен успешно");
    bool x = dbConnect.makeNote( 1, getCurrentDateAndTime(), 1, data, 2);
    if ( x ) {
        makeLogNote( "запись действия добавлена в БД" );
    }
        else {
        makeLogNote( "ошиба записи действия в БД" );
    }
}

QString MainWindow::makeDatagramCoord( QString q )
{
    QString answer = "";
    answer.append( "0" );                        //метод сжатия
    answer.append( converter->dobei( q, 6 ) );      //отправитель добить до 6
    answer.append( converter->dobei( "mbu" , 6) );  //получатель
    answer.append( "0" );                        //категория данных
    answer.append( "C" );                        //данные о сообщении
    answer.append( "C1" );                       //Идентификатор приложения, которое  должно обрабатывать переданные данные.
    answer.append( "=" );                        //Признак начала передаваемых данных
    QString request = dbConnect.getCoordInformation(q);
    if (request.compare("error") == 0) {
        return "error";
    }
    else {
        answer.append(request);
    }
    return answer;
}

QString MainWindow::makeDatagramRocket( QString q )
{
    QString answer = "";
    answer.append( "0" );                        //метод сжатия
    answer.append( converter->dobei( q , 6 ) );      //отправитель добить до 6
    answer.append( converter->dobei( "mbu" , 6) );  //получатель
    answer.append( "0" );                        //категория данных
    answer.append( "C" );                        //данные о сообщении
    answer.append( "T1" );                       //Идентификатор приложения, которое  должно обрабатывать переданные данные.
    answer.append( "=" );                        //Признак начала передаваемых данных
    QString request = dbConnect.getRocketInformation(q);
    if (request.compare("error") == 0) {
        return "error";
    }
    else {
        answer.append(request);
    }
    return answer;
}

void MainWindow::setTargetIp() {
    IpDialog dia;
    if ( dia.exec() ) {
        targetIp = dia.value();
    }
}

void MainWindow::setMyIp() {
    IpDialog dia;
    dia.changeTitle();
    if ( dia.exec() ) {
        myIp = dia.value();
    }
}

void MainWindow::readDatagram()
{
    QByteArray datagram;
    QByteArray package;
    QByteArray confirm;
    datagram.resize( udpSocket.pendingDatagramSize() );
    udpSocket.readDatagram( datagram.data(), datagram.size() );
    qDebug() << datagram;
    qDebug() << datagram.size();
    QStringList messageMembersList = converter->decodeDatagram(datagram, &package, &confirm);
    qDebug() << "package" << package;
    parsingMessage( package );
}

void MainWindow::sendConfirm(QByteArray datagram) {
    udpSocket.writeDatagram( datagram, targetIp, targetPort.toLong( Q_NULLPTR, 10) );
    makeLogNote( "отправлен пакет подтверждения" );
}

void MainWindow::parsingMessage( QByteArray s )
{
    QString source = "";
    for (int i=0; i<1; i++) {
        QByteArray q;
        q[0]=s[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "метод сжатия данных " << source;
    source = "";
    for (int i=1; i<7; i++) {
        QByteArray q;
        q[0]=s[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "отправитель " << source;
    source = "";
    for (int i=7; i<13; i++) {
        QByteArray q;
        q[0]=s[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "получатель " << source;
    source = "";
    for (int i=13; i<14; i++) {
        QByteArray q;
        q[0]=s[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "категория данных " << source;
    source = "";
    for (int i=14; i<15; i++) {
        QByteArray q;
        q[0]=s[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "тип протокола " << source;
    source = "";
    for (int i=15; i<17; i++) {
        QByteArray q;
        q[0]=s[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "тип сообщения " << source;
    if ( s.at( 0 ) == '1' ) {
        makeLogNote( "oшибка, данные сжаты" );
        //не работаем пока со сжатием данных
        return;
    }
    bool trigger = false;
    QString object = "";
    QString messageCode=source;
    if (QString::compare( messageCode, "K1") == 0) {
        parsingCommand(s);
        if ( dbConnect.makeNote( 1, getCurrentDateAndTime(), 1, s, 3 ) ) {
            makeLogNote( "получена КОМАНДА" );
        }
            else {
            makeLogNote( "ошибка занесения в лог" );
        }
    }
    else {
        bool x = dbConnect.makeNote( 1, getCurrentDateAndTime(), 1, s, 3 );
        if ( x ) {
            makeLogNote( "получена датаграмма квитанция" );

        }
            else {
            makeLogNote( "FAIL1" );
        }
    }
}

void MainWindow::parsingCommand( QByteArray s)
{
    QByteArray data = "";
    CommandsMessageBox box;
    for ( int i = 18; i < s.size(); i++ )
    {
        data += s.at( i );
    }
    qDebug() << data;
    QString source = "";
    for (int i=0; i<6; i++) {
        QByteArray q;
        q[0]=data[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "идентификатор команды " << source;
    box.setIdCommand(source);
    source = "";
    for (int i=7; i<11; i++) {
        QByteArray q;
        q[0]=data[i];
        source.append(converter->convertToBinaryNew(converter->convertByteToDec(q)));
        qDebug() << source;
    }
    source = converter->convertToDex(source);
    qDebug() << "время формирования команды " << source;
    box.setTimeAdd(source);
    source = "";
    for (int i=12; i<14; i++) {
        QByteArray q;
        q[0]=data[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "наименование команды " << source;
    box.setCommandName(source);
    source = "";
    for (int i=15; i<19; i++) {
        QByteArray q;
        q[0]=data[i];
        source.append(converter->convertToBinaryNew(converter->convertByteToDec(q)));
        qDebug() << source;
    }
    source = converter->convertToDex(source);
    qDebug() << "время исполнения команды " << source;
    box.setTimeExec(source);
    source = "";
    for (int i=20; i<21; i++) {
        QByteArray q;
        q[0]=data[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "признак исполнения " << source;
    box.setAttributeExec(source);
    source = "";
    for (int i=22; i<23; i++) {
        QByteArray q;
        q[0]=data[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "Наименование параметра " << source;
    QStringList paramList;
    QStringList paramValueList;
    paramList << source;
    source = "";
    for (int i=24; i<27; i++) {
        QByteArray q;
        q[0]=data[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "Значение параметра " << source;
    paramValueList << source;
    source = "";
    for (int i=28; i<data.size()-1; i++) {
        QByteArray q;
        q[0]=data[i];
        QString str(q);
        source.append(str);
    }
    qDebug() << "уточняющий текст " << source;
    box.setParametrs(paramList);
    box.setParametrsValue(paramValueList);
    if ( dbConnect.saveCommand("1.11", box)) {
            makeLogNote( "база обновлена" );
        }
        else {
            makeLogNote( "ошибка добавления комманды" );
        }
}

QString MainWindow::assistParser( QString data, int &counter )
{
    QString answer = "";
    while ( data.at( counter ) != ';' && data.at( counter ) != '\r' ) {
        answer.append( data.at( counter ) );
        counter++;
    }
    counter++;
    return answer;
}

void MainWindow::on_logTableBut_3_clicked()
{
    /*QSqlTableModel *model = dbConnect.getTable(ui->tableView, "log.log_table_message", "log_table_message");
    ui->tableView->setModel( model );
    for ( int i = 0; i < model->columnCount(); i++ ) {
        ui->tableView->horizontalHeader()->setSectionResizeMode( i , QHeaderView::ResizeToContents);
    }
    makeLogNote( "Загружены данные log table" );*/
}

QString MainWindow::getCurrentDateAndTime()
{
    return QDate::currentDate().toString( "dd.MM.yyyy" ) + " " + QTime::currentTime().toString( "hh:mm:ss.zzz" );
}
