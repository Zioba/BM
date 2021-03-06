#ifndef CONVERTER_H
#define CONVERTER_H

#include <QString>
#include <QStringList>
#include <QDebug>


class Converter
{
public:
    Converter();

    QByteArray encodeDatagram( QStringList ); //закодировать датаграмму
    QStringList decodeDatagram(QByteArray , QByteArray *, QByteArray *confirm); //раскодировать датаграмму
    QString dobei( QString, int );  //приведение длины строки к нужному формату
    QString convertIpBinary( QString ); //конвертирует обычный ip адрес в бинарную строку
    QString convertToBinary( QString, int ); //преобразование заданной строки в бинарную, и подгонка её к нужному размеру
    QString convertIpDex( QString ); //преобразование бинарной строки в обычный ip адрес
    QString convertToDex( QString ); //преобразование бинарной строки в обычную
    QString convertToBinaryNew(QString s);
    QString convertByteToDec(QByteArray q);
    QByteArray convertToHex(QString s);
    QByteArray convertIpHex(QString s);
    QByteArray convertToDoubleHex(QString s);
};

#endif // CONVERTER_H
