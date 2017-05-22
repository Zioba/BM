#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>
#include <QDateEdit>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlRecord>
#include "converter.h"
#include "dbworker.h"
#include "commandsaddform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DbWorker dbConnect, QWidget *parent = 0);
    QByteArray getInformationAboutObj( QByteArray );
    void makeLogNote( QString );

     ~MainWindow();

private slots:
    void readDatagram();
    void on_exitButton_clicked();
    void on_updBut_clicked();
    void on_clearBut_clicked();
    void on_itemSendCoord_triggered();
    void on_itemSendRocket_triggered();
    void setTargetIp();
    void setMyIp();
    void on_combObjTableBut_clicked();
    void on_logTableBut_3_clicked();

    QString getCurrentDateAndTime();
    QString makeDatagramCoord( QString );
    QString makeDatagramRocket( QString q );

private:
    const int X_POSITION_FRAME=0;
    const int Y_POSITION_FRAME=1;
    const int LISTERNING_PORT=5825;
    int unicumMessageId = 1;

    const QString myPort = "5825";
    const QString targetPort = "5824";

    Ui::MainWindow *ui;
    QUdpSocket udpSocket;
    QHostAddress myIp;
    QHostAddress targetIp;
    Converter *converter;
    DbWorker dbConnect;
    void parsingMessage(QString s);
    QString assistParser(QString data, int &counter);
    void parsingCommand(QString s, QString object);
};

#endif // MAINWINDOW_H
