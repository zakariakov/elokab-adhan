/***************************************************************************
 *   elokab Copyright (C) 2014 AbouZakaria <yahiaui@gmail.com>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtSql>
#include "databaseadhkar.h"
#include<QApplication>
#include<QDebug>
DataBaseAdhkar::DataBaseAdhkar()
{
    m_curent=0;
}
bool DataBaseAdhkar::checkDatabase()
{
    QDir appDir(QApplication::applicationDirPath());

#ifdef Q_OS_HAIKU
    appDir.cd(".");
    dbFilePath.setFileName(appDir.absolutePath()+"/data/adhkar.db");
#else
    appDir.cdUp();

    dbFilePath.setFileName(appDir.absolutePath()+"/share/elokab/elokab-adhan/data/adhkar.db");
#endif
    if ( dbFilePath.exists() )
        return true;

    return false;
}

bool DataBaseAdhkar::makeConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(dbFilePath.fileName());
    db.setUserName("");
    db.setPassword("");

    if ( db.isOpen() || !db.open())
        return false;

    countRecord();
    return true;
}

void DataBaseAdhkar::closeConnection()
{
    db.removeDatabase(db.connectionName());
}
void DataBaseAdhkar::countRecord()
{
   QSqlQuery query("select * from athkar");

    query.last();
   m_count=query.at();
   qDebug()<<"m_count"<<m_count;
    //   query.exec("create table athkar ("
    //              "title varchar, nass varchar)");
  //  query.seek(1);
  //  qDebug()<<query.value(0).toString();

}
QStringList DataBaseAdhkar::getDhikr(bool order)
{

    QStringList list;
    QSqlQuery query("select * from athkar");
     qDebug()<<"order"<<query.record().count();
    if(query.record().count()<1)return list;
    if(order==true){
        query.seek(m_curent);

        list.append( query.value(0).toString());
        list.append( query.value(1).toString());
        m_curent++;
    }else{
        int rnd=m_count;
        int rndX=rand() % ++rnd;
         query.seek(rndX);
         list.append( query.value(0).toString());
         list.append( query.value(1).toString());
         qDebug()<<rndX;
         m_curent=rndX+1;

    }

    if(m_curent>m_count-1)
        m_curent=0;

    return list;
}
