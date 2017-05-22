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
    void setCommandsSignals(QString s);
    void setAttributeExecution(QString s);
    void setParametr(QString parametr, QString value);
    void setTimeCreate(QString s);
    void setTimeExecution(QString s);

private:
};

#endif // COMMANDSADDFORM_H
