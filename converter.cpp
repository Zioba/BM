#include "converter.h"

Converter::Converter()
{

}

QByteArray Converter::encodeDatagram( QStringList list )
{
    QByteArray datagram;
    datagram.append(convertIpHex(list.at(0)));        //адрес источника
    datagram.append(convertIpHex(list.at(1)));        //адрес получателя
    datagram.append(convertToHex(list.at(2)));        //нули
    datagram.append(convertToHex(list.at(3)));        //номер протокола
    datagram.append(convertToDoubleHex(list.at(4)));        //длина UDP
    datagram.append(convertToDoubleHex(list.at(5)));  //порт источника
    datagram.append(convertToDoubleHex(list.at(6)));  //порт получателя
    datagram.append(convertToDoubleHex(list.at(7)));  //длина
    datagram.append(convertToDoubleHex(list.at(8)));  //чексумма
    QString s;
    s.append(list.at(9));
    s.append(convertToBinary(list.at(10), 28));
    QString part1;
    QString part2;
    for (int i = 0; i < s.size(); i++) {
        if (i < s.size()/2) {
            part1.append(s.at(i));
        }
        else {
            part2.append(s.at(i));
        }
    }
    datagram.append( convertToDoubleHex(part1));      //тип сообщения
    datagram.append( convertToDoubleHex(part2));      //УНИКАЛЬНЫЙ номер
    datagram.append(convertToDoubleHex(list.at(11))); //всего датаграмм
    datagram.append(convertToDoubleHex(list.at(12))); //номер в сообщении
    /*qDebug() << datagram;
    for (int i = 0; i < datagram.size(); i++) {
        QByteArray q;
        q[0] = datagram[i];
       qDebug() << q << datagram[i] << QByteArray::fromHex(q);
    }*/
    datagram.append(list.at(13));                      //данные
    return datagram;
}

QStringList Converter::decodeDatagram(QByteArray datagram, QByteArray *package, QByteArray *confirm)
{
    QStringList list;
    QString source = "";
    for (int i=4; i<8; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertByteToDec(q));
        if (i != 7) source.append(".");
    }
    qDebug() << "адрес приемника " << source;
    /*2*/list << source;
    source = "";
    for (int i=0; i<4; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertByteToDec(q));
        if (i != 3) source.append(".");
    }
    qDebug() << "адрес источника " << source;
    /*1*/list << source;
    source = "";
    for (int i=8; i<9; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertByteToDec(q));
    }
    qDebug() << "нули " << source;
    /*3*/list << source;
    source = "";
    for (int i=9; i<10; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertByteToDec(q));
    }
    qDebug() << "протокол " << source;
    /*4*/list << source;
    source = "";
    for (int i=10; i<12; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertToBinaryNew(convertByteToDec(q)));
    }
    source = convertToDex(source);
    qDebug() << "длина UDP " << source;
    /*5*/list << source;
    source = "";
    for (int i=12; i<14; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertToBinaryNew(convertByteToDec(q)));
    }
    qDebug() << source;
    source = convertToDex(source);
    qDebug() << "порт источника " << source;
    /*6*/list << source;
    source = "";
    for (int i=14; i<16; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertToBinaryNew(convertByteToDec(q)));
    }
    source = convertToDex(source);
    qDebug() << "порт получателя " << source;
    /*7*/list << source;
    source = "";
    for (int i=16; i<18; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertToBinaryNew(convertByteToDec(q)));
    }
    source = convertToDex(source);
    qDebug() << "длина " << source;
    /*8*/list << source;
    source = "";
    for (int i=18; i<20; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertToBinaryNew(convertByteToDec(q)));
    }
    source = convertToDex(source);
    qDebug() << "контрольная сумма " << source;
    /*9*/list << source;
    source = "";
    for (int i=20; i<22; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertToBinaryNew(convertByteToDec(q)));
    }
    source = convertToDex(source);
    qDebug() << "НЕ СДЕЛАНО " << source;
    /*10*/list << source;
    source = "";
    for (int i=22; i<24; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertToBinaryNew(convertByteToDec(q)));
    }
    source = convertToDex(source);
    qDebug() << "НЕ СДЕЛАНО " << source;
    /*11*/list << source;
    source = "";
    for (int i=24; i<26; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertToBinaryNew(convertByteToDec(q)));
    }
    source = convertToDex(source);
    qDebug() << "всего датаграмм в сообщении " << source;
    /*12*/list << source;
    source = "";
    for (int i=26; i<28; i++) {
        QByteArray q;
        q[0]=datagram[i];
        source.append(convertToBinaryNew(convertByteToDec(q)));
    }
    source = convertToDex(source);
    qDebug() << "номер датаграммы в сообщении " << source;
    /*13*/list << source;
    source = "";
    for (int i=28; i<datagram.size(); i++) {
        QByteArray q;
        q[0]=datagram[i];
        QString str(q);
        source.append(str);
        package->append(datagram[i]);
    }
    qDebug() << "ДАННЫЕ " << source;
    qDebug() << "datagramma " << package;
    /*14*/list << source;
    /*QString source( datagram );
    list << convertIpDex( source.mid( 0, 32 ) );
    list << convertIpDex( source.mid( 32, 32 ) );
    list << convertToDex( source.mid( 64, 16 ) );
    list << convertToDex( source.mid( 80, 16 ) );
    list << convertToDex( source.mid( 96, 16 ) );
    list << convertToDex( source.mid( 112, 16 ) );
    list << convertToDex( source.mid( 128, 16 ) );
    list << convertToDex( source.mid( 144, 16 ) );
    list << source.mid( 160, 4 );
    list << convertToDex( source.mid( 164, 28 ) );
    list << convertToDex( source.mid( 192, 16 ) );
    list << convertToDex( source.mid( 208, 16 ) );
    list << source.mid( 224, datagram.size() );*/
    return list;
}

QString Converter::dobei( QString s, int len )
{
    while ( s.size() < len ) {
        s = "0" + s;
    }
    return s;
}

QString Converter::convertIpBinary( QString s )
{
    QString answer = "";
    int i = 0;
    for ( int j = 0; j < 4; j++ ) {
        QString source = "";
        while ( ( i < s.size() ) && ( s.at( i ) != '.' ) ) {
            source.append( s.at( i ) );
            i++;
        }
        i++;
        answer.append( convertToBinary( source, 8 ) );
    }
    return answer;
}

QString Converter::convertToBinary( QString s, int len )
{
    long long answer = s.toLong( Q_NULLPTR, 10 );
    QString b2 = QString( "%1" ).arg( answer, 0, 2 );
    while ( b2.size() < len ) {
        b2 = "0" + b2;
    }
    return b2;
}

QString Converter::convertToBinaryNew(QString s)
{
    long long answer = s.toLong( Q_NULLPTR, 10 );
    QString b2 = QString( "%1" ).arg( answer, 0, 2 );
    return b2;
}

QString Converter::convertIpDex( QString s )
{
    QString answer = "";
    int i = 0;
    for ( int j = 0; j < 4; j++ ) {
        QString source = "";
        source.append( s.at( i ) );
        i++;
        while ( i % 8 != 0 ) {
            source.append( s.at( i ) );
            i++;
        }
        answer.append( convertToDex( source ) );
        if ( i < 32 ) answer.append( "." );
    }
    return answer;
}

QString Converter::convertToDex( QString s )
{
    long long answer = s.toLong( Q_NULLPTR, 2 );
    QString b2 = QString( "%1" ).arg( answer, 0, 10 );
    return b2;
}

QString Converter::convertByteToDec(QByteArray q) {
    QString str(q.toHex());
    uint x = str.toUInt(NULL, 16);
    return QString::number(x);
}

QByteArray Converter::convertToHex( QString s)
{
    long long answer = s.toLong(Q_NULLPTR, 10);
    QString b2 = QString("%1").arg(answer, 0, 16).toUpper();
    QByteArray q;
    q.append(b2);
    return QByteArray::fromHex(q);
}

QByteArray Converter::convertIpHex(QString s) {
    QByteArray answer = "";
    int i = 0;
    for (int j = 0; j < 4; j++) {
        QString source = "";
        while ((i < s.size()) && (s.at(i) != '.')) {
            source.append(s.at(i));
            i++;
        }
        i++;
        answer.append(convertToHex(source));
    }
    return answer;
}

QByteArray Converter::convertToDoubleHex( QString s)
{
    QByteArray answer;
    QString material = convertToBinary(s,16);
    QString s1 = "";
    QString s2 = "";
    for (int i = 0; i < material.size(); i++) {
        if (i<material.size()/2) {
            s1.append(material.at(i));
        }
        else {
            s2.append(material.at(i));
        }
    }
    QString q = convertToDex(s1);
    answer.append(convertToHex(q));
    q = convertToDex(s2);
    answer.append(convertToHex(q));
    return answer;
}
