#include "dbworker.h"

DbWorker::DbWorker( QString hostName, int port, QString baseName,
                    QString userName, QString password )
{
    db = QSqlDatabase::addDatabase( "QPSQL" );
    db.setHostName( hostName );
    db.setPort( port );
    db.setDatabaseName( baseName );
    db.setUserName( userName );
    db.setPassword( password );
    if ( !db.open() ) {
        connectionStatus = false;
    }
    else {
        connectionStatus = true;
    }
}

bool DbWorker::getConnectionStatus()
{
    return connectionStatus;
}

bool DbWorker::makeNote( int type, QString date, int x, QString package, int status )
{
    QSqlQuery query = QSqlQuery( db );
    QString s;
    s = "SELECT id_note FROM log.log_table_message ORDER BY id_note DESC LIMIT 1;";
    if ( !query.exec( s ) ) {
        return false;
    }
    else {
        int id;
        while ( query.next() ) {
            id = query.value( 0 ).toInt();
        }
        id++;
        db.transaction();
        query.prepare( "INSERT INTO log.log_table_message(id_note, type_message, date, reciver_sender, package, status)"
                                      "VALUES (?, ?, ?, ?, ?, ?)" );
        query.addBindValue( id );
        query.addBindValue( type );
        query.addBindValue( date );
        query.addBindValue( x );
        query.addBindValue( package );
        query.addBindValue( status );
        query.exec();
        return db.commit();
    }
    return false;
}

QSqlTableModel *DbWorker::getTable(QTableView *table, QString tableName, QString shortName) {
    QSqlTableModel *model = new QSqlTableModel(table, db);
    model->setTable(tableName);
    model->select();
    QSqlQuery query = QSqlQuery(db);
    query.prepare( "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '"+shortName+"';" );
    query.exec();
    if ( query.next() ){
        for ( int i = 0; i < query.size(); i++) {
            QSqlRecord rec;
            rec = query.record();
            QSqlQuery query2 = QSqlQuery(db);
            QString s = "SELECT description FROM pg_description INNER JOIN ";
            query2.prepare( s +
                            "(SELECT oid FROM pg_class WHERE relname ='" +
                            shortName +
                            "') as table_oid " +
                            "ON pg_description.objoid = table_oid.oid " +
                            "AND pg_description.objsubid IN " +
                            "(SELECT attnum FROM pg_attribute WHERE attname = '" +
                            query.value(0).toString() +
                            "' AND pg_attribute.attrelid = table_oid.oid );" );
            query2.exec();
            if (query2.next()) {
                model->setHeaderData(i, Qt::Horizontal, query2.value(0).toString());
            }
            query.next();
        }
    }
    return model;
}

QString DbWorker::getCoordInformation(QString object)
{
    QString answer = "";
    QSqlQuery query= QSqlQuery(db);
    QString s;
    s = "SELECT st_x(obj_location), st_y(obj_location), st_z(obj_location), direction FROM own_forces.combatobject_location WHERE combat_hierarchy='"+object+"';";
    if ( !query.exec( s ) ) {
        return "error";
    }
    else {
        if ( query.size() == 0 ) return "error";
        while ( query.next() ) {
            answer.append( query.value(0).toString() );
            answer.append(";");
            answer.append( query.value(1).toString() );
            answer.append( ";" );
            answer.append( query.value(2).toString() );
            answer.append(";");
            answer.append( query.value(3).toString() );
            answer.append(";");
        }
    }
    answer.append( "\r" );
    return answer;
}

QString DbWorker::getRocketInformation(QString object)
{
    QString answer = "";
    QSqlQuery query= QSqlQuery(db);
    QString s;
    s = "SELECT type_tid FROM own_forces.rocket WHERE combatobjectid='"+object+"';";
    if ( !query.exec( s ) ) {
        return "error";
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
    answer.append( "\r" );
    return answer;
}

bool DbWorker::writeCoordinats(QString x, QString y, QString z, QString direction,
                               QString time, QString object)
{
    QSqlQuery query = QSqlQuery(db);
    QString queryString;
    queryString = "UPDATE own_forces.combatobject_location SET obj_location=ST_MakePoint ("+x+","+y+","+z+"), direction='"+
            direction+"', date_edit='"+ time + "' WHERE combat_hierarchy='"+object+"';";
    if (query.exec(queryString)) {
        return true;
    }
    else {
        return false;
    }
}

bool DbWorker::writeRocket(QString x,QString time, QString object)
{
    QSqlQuery query = QSqlQuery(db);
    QString queryString;
    queryString = "UPDATE own_forces.rocket SET type_tid='"+ x +
            "', date_edit='"+ time + "' WHERE combatobjectid='"+object+"';";
    if ( query.exec(queryString) ) {
        return true;
    }
    else {
        return false;
    }
}

QString DbWorker::convertReferenceNameTOCode(QString referenceName)
{
    QSqlQuery query = QSqlQuery( db );
    QString s = "";
    QString code;
    s = "SELECT reference_data.terms.termhierarchy FROM reference_data.terms WHERE reference_data.terms.termname ='"+referenceName+"';";
    if (!query.exec(s)) {
        return "";
    }
    else {
            while ( query.next() ) {
                code = query.value( 0 ).toString();
            }
    }
    return code;
}

QString DbWorker::convertCodeToReferenceName(QString code)
{
    QSqlQuery query = QSqlQuery( db );
    QString s = "";
    QString referenceName;
    s = "SELECT reference_data.terms.termname FROM reference_data.terms WHERE reference_data.terms.termhierarchy ='"+code+"';";
    if (!query.exec(s)) {
        return "";
    }
    else {
            while ( query.next() ) {
                referenceName = query.value( 0 ).toString();
            }
    }
    return referenceName;
}
