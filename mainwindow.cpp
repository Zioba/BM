#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deldialog.h"
#include "ipdialog.h"
#include <QErrorMessage>
#include <QMessageBox>

MainWindow::MainWindow(DbWorker dbConnect, QWidget *parent) :
    dbConnect(dbConnect),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QPoint pos( X_POSITION_FRAME, Y_POSITION_FRAME );
    this->move( pos );
    ui->setupUi( this );
    setWindowTitle("BM");
    makeLogNote("Start working");
    udpSocket.bind( LISTERNING_PORT );
    on_combObjTableBut_clicked();
    converter = new Converter();
    setTargetIp();
    setMyIp();
    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(readDatagram()));
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
//QSqlTableModel *model;
void MainWindow::on_updBut_clicked()
{
    ui->tableView->update();
    ui->logField->append( tr( "%1 таблица обновлена" ).arg( QTime::currentTime().toString( "hh:mm:ss" ) ) );
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
         << "17"
         << QString::number( data.length() + 224 )
         << myPort
         << targetPort
         << QString::number( data.length() )
         << ""
         << "0001"
         << QString::number( unicumMessageId )
         << "1"
         << "1"
         << data;
    unicumMessageId++;
    QByteArray datagram = converter->encode( list );
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
         << "17"
         << QString::number( data.length() + 224 )
         << myPort
         << targetPort
         << QString::number( data.length() )
         << ""
         << "0001"
         << QString::number( unicumMessageId )
         << "1"
         << "1"
         << data;
    unicumMessageId++;
    QByteArray datagram = converter->encode( list );
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
    QSqlQuery query= QSqlQuery( db );
    QString s;
    s = "SELECT st_x(obj_location), st_y(obj_location), st_z(obj_location), direction FROM own_forces.combatobject_location WHERE combat_hierarchy='"+q+"';";
    if ( !query.exec( s ) ) {
        makeLogNote("cant select");
    }
    else {
        if ( query.size() == 0 ) return "error";
        while ( query.next() ) {
            answer.append( query.value( 0 ).toString() );
            answer.append( ";" );
            answer.append( query.value( 1 ).toString() );
            answer.append( ";" );
            answer.append( query.value( 2 ).toString() );
            answer.append( ";" );
            answer.append( query.value( 3 ).toString() );
            answer.append( ";" );
        }
    }
    answer.append( "\r" );
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
    QSqlQuery query= QSqlQuery( db );
    QString s;
    s = "SELECT type_tid FROM own_forces.rocket WHERE combatobjectid='"+q+"';";
    if ( !query.exec( s ) ) {
        makeLogNote("cant select");
    }
    else {
        if ( query.size() == 0 ) return "error";
        while ( query.next() ) {
            if (QString::compare( query.value( 0 ).toString(), "51.50.10") == 0) {
                answer.append( "11" );
                answer.append( ";" );
            }
            if (QString::compare( query.value( 0 ).toString(), "51.50.15") == 0) {
                answer.append( "12" );
                answer.append( ";" );
            }
            if (QString::compare( query.value( 0 ).toString(), "51.50.20") == 0) {
                answer.append( "13" );
                answer.append( ";" );
            }
            if (QString::compare( query.value( 0 ).toString(), "51.50.25") == 0) {
                answer.append( "14" );
                answer.append( ";" );
            }
            if (QString::compare( query.value( 0 ).toString(), "51.50.30") == 0) {
                answer.append( ";" );
            }
        }
    }
    qDebug() << answer;
    answer.append( "\r" );
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
    datagram.resize( udpSocket.pendingDatagramSize() );
    udpSocket.readDatagram( datagram.data(), datagram.size() );
    qDebug() << datagram;
    QStringList list=converter->decode( datagram );
    bool x = dbConnect.makeNote( 1, getCurrentDateAndTime(), 1, list.at( 12 ), 3 );
    if ( x ) {
        makeLogNote( "получена датаграмма квитанция" );

    }
        else {
        makeLogNote( "FAIL1" );
    }
}

void MainWindow::on_combObjTableBut_clicked()
{
    QSqlTableModel *model = dbConnect.getTable(ui->tableView, "own_forces.combatobject_location", "combatobject_location");
    ui->tableView->setModel( model );
    for ( int i = 0; i < model->columnCount(); i++ ) {
        ui->tableView->horizontalHeader()->setSectionResizeMode( i , QHeaderView::ResizeToContents);
    }
    makeLogNote( "Загружены данные combat objects" );
}

void MainWindow::on_logTableBut_3_clicked()
{
    QSqlTableModel *model = dbConnect.getTable(ui->tableView, "log.log_table_message", "log_table_message");
    ui->tableView->setModel( model );
    for ( int i = 0; i < model->columnCount(); i++ ) {
        ui->tableView->horizontalHeader()->setSectionResizeMode( i , QHeaderView::ResizeToContents);
    }
    makeLogNote( "Загружены данные log table" );
}

QString MainWindow::getCurrentDateAndTime()
{
    return QDate::currentDate().toString( "dd.MM.yyyy" ) + " " + QTime::currentTime().toString( "hh:mm:ss.zzz" );
}
