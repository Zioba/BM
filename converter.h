#ifndef CONVERTER_H
#define CONVERTER_H

#include <QString>
#include <QStringList>
#include <QDebug>


class Converter
{
public:
    Converter();

    QByteArray encode( QStringList ); //закодировать датаграмму

    QStringList decode( QByteArray ); //раскодировать датаграмму

    QString dobei( QString, int );  //приведение длины строки к нужному формату

private:
    QString convertIpBinary( QString ); //конвертирует обычный ip адрес в бинарную строку

    QString convertToBinary( QString, int ); //преобразование заданной строки в бинарную, и подгонка её к нужному размеру

    QString convertIpDex( QString ); //преобразование бинарной строки в обычный ip адрес

    QString convertToDex( QString ); //преобразование бинарной строки в обычную
};

#endif // CONVERTER_H
