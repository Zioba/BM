#include "commandsaddform.h"
#include "ui_commandsaddform.h"

CommandsAddForm::CommandsAddForm(QString ownName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommandsAddForm)
{
    ui->setupUi(this);
    ui->dataSourceLine->setText(ownName);
    ui->timeCreateDTE->setDateTime(QDateTime::currentDateTime());
    ui->timeExecDTE->setDateTime(QDateTime::currentDateTime());
    ui->timeDocRegister->setDateTime(QDateTime::currentDateTime());
    ui->stackedWidget->setCurrentIndex(0);
    ui->tableWidget_4->setRowCount(0);
}

CommandsAddForm::~CommandsAddForm()
{
    delete ui;
}

void CommandsAddForm::setParametr(QString parametr, QString value)
{
    int n = ui->tableWidget_4->rowCount();
    if (n == 0) {
        ui->tableWidget_4->setRowCount(1);
    }
    else {
        ui->tableWidget_4->insertRow(n);
    }
    n = ui->tableWidget_4->rowCount();
    ui->tableWidget_4->setItem(n-1, 0, new QTableWidgetItem(parametr));
    ui->tableWidget_4->setItem(n-1, 1, new QTableWidgetItem(value));
}

void CommandsAddForm::setTimeCreate(QString s)
{
    QDateTime *trash = new QDateTime();
    trash->fromString(s,"yyyy-mm-dd hh:mm:ss");
    //2000-12-31 23:00:33.916+02
    ui->timeCreateDTE->setDateTime(*trash);
}

void CommandsAddForm::setTimeExecution(QString s)
{
    //ui->timeExecDTE->setTime(s);
}

void CommandsAddForm::setCommandsSignals(QString s)
{
    ui->commandsSignalsLine->setText(s);
}

void CommandsAddForm::setAttributeExecution(QString s)
{
    ui->attrExecLine->setText(s);
}

void CommandsAddForm::on_endBut_clicked()
{
   this->close();
}
