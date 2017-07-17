#ifndef COMMANDSMESSAGEBOX_H
#define COMMANDSMESSAGEBOX_H

#include <QStringList>

class CommandsMessageBox
{
public:
    CommandsMessageBox();

    QString getCommandName() const;
    void setCommandName(const QString &value);
    QString getTimeAdd() const;
    void setTimeAdd(const QString &value);
    QString getTimeExec() const;
    void setTimeExec(const QString &value);
    QString getAttributeExec() const;
    void setAttributeExec(const QString &value);
    QStringList getParametrs() const;
    void setParametrs(const QStringList &value);
    QStringList getParametrsValue() const;
    void setParametrsValue(const QStringList &value);
    QStringList getReceivers() const;
    void setReceivers(const QStringList &value);
    QStringList getReceiversMarks() const;
    void setReceiversMarks(const QStringList &value);
    QStringList getReceiversTime() const;
    void setReceiversTime(const QStringList &value);
    QString getIdCommand() const;
    void setIdCommand(const QString &value);

private:
    QString commandName;
    QString timeAdd;
    QString timeExec;
    QString AttributeExec;
    QStringList parametrs;
    QStringList parametrsValue;
    QStringList receivers;
    QStringList receiversMarks;
    QStringList receiversTime;
    QString idCommand;
};

#endif // COMMANDSMESSAGEBOX_H
