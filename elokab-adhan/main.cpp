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

#include <QApplication>
#include "adhan.h"
#include <QMessageBox>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setQuitOnLastWindowClosed(false);


    a.setApplicationName("elokab-adhan");
    a.setApplicationVersion("0.1");
    a.setOrganizationName("elokab");
    a.setWindowIcon(QIcon::fromTheme("elokab-adhan",QIcon(":/images/elokab-adhan.png")));

    /// مسار مجلد البرنامج
    QDir appDir(a.applicationDirPath());
    QString translatPath;
    appDir.cdUp();
    translatPath=  appDir.absolutePath()+"/share/elokab/translations";
    /// جلب اعدادات اللغة
    QSettings globalSetting("elokab","elokabsettings");

    globalSetting.beginGroup("Language");
    QString locale=globalSetting.value("Name","Default").toString();
    globalSetting.endGroup();

qDebug()<<globalSetting.fileName()<<QLocale::system().name();
    /// اذاكانت اللغة الافتراضية
    if(locale=="Default")
            locale = QLocale::system().name().section("_",0,0);


    /// اللغة الحالية لجميع البرنامج
  QLocale::setDefault(QLocale(locale));
    /// جلب ترجمات كيوتي
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += locale;
    QTranslator *translatorsys = new QTranslator;
    if (translatorsys->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
            QApplication::installTranslator(translatorsys);
    /// جلب ترجمة البرنامج من مجلد البرنامج
    QTranslator translator;
    translator.load(QString(translatPath+"/"+locale+"/"+a.applicationName()));

    a.installTranslator(&translator);
qDebug()<<translatPath+"/"+locale+"/"+a.applicationName();
if(locale=="ar")
   a.setLayoutDirection(Qt::RightToLeft);
    Adhan w;
    w.hide();
    
    return a.exec();
}
