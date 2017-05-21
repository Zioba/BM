#ifndef COMMANDSADDFORM_H
#define COMMANDSADDFORM_H

#include <QWidget>
#include <QString>
#include <QtSql>
#include <QTreeWidget>
#include <QDateTime>

namespace Ui {
class CommandsAddForm;
}

class CommandsAddForm : public QWidget
{
    Q_OBJECT

public:
    explicit CommandsAddForm(QString ownName, QWidget *parent = 0);
    ~CommandsAddForm();
    Ui::CommandsAddForm *ui;

private:

    void setCommandsSignals();
    void setAttributeExecution();

private slots:
    void changeEnabledTimerExec();
    void addRecivers();
    void receiveDataParametrs(QString parametr, QString value);
};

#endif // COMMANDSADDFORM_H
