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
    setCommandsSignals();
    setAttributeExecution();

    connect(ui->timeExecBox, SIGNAL(clicked(bool)), this, SLOT(changeEnabledTimerExec()));
}

CommandsAddForm::~CommandsAddForm()
{
    delete ui;
}

void CommandsAddForm::addRecivers()
{

}

void CommandsAddForm::receiveDataParametrs(QString parametr, QString value)
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

void CommandsAddForm::setCommandsSignals()
{
    QSqlQuery query;
    QString selectQuery = "SELECT termname FROM reference_data.terms WHERE termhierarchy ~ '70.10.*{1,}';";
    if (!query.exec(selectQuery)) {
        qDebug() << "Unable to make select operation!" << query.lastError();
    }

    QStringList list;
    while (query.next()) {
        list.append(query.value(0).toString());
    }
    ui->commandsSignalsBox->addItems(list);
}

void CommandsAddForm::setAttributeExecution()
{
    QSqlQuery query;
    QString selectQuery = "SELECT termname FROM reference_data.terms WHERE termhierarchy ~ '70.30.*{1,}';";
    if (!query.exec(selectQuery)) {
        qDebug() << "Unable to make select operation!" << query.lastError();
    }

    QStringList list;
    while (query.next()) {
        list.append(query.value(0).toString());
    }
    ui->attrExecBox->addItems(list);
}

void CommandsAddForm::changeEnabledTimerExec()
{
    if ( ui->timeExecBox->isChecked() ) {
        ui->timeExecDTE->setEnabled(true);
    }
    else {
        ui->timeExecDTE->setEnabled(false);
    }
}
