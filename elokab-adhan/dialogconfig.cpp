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

#include "dialogconfig.h"
#include "ui_dialogconfig.h"
#include<QSettings>
DialogConfig::DialogConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfig)
{

   this->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->setupUi(this);
    QListWidgetItem *locale = new QListWidgetItem(QObject::tr("Location"),ui->listWidget);
    locale->setIcon(QIcon::fromTheme("preferences-desktop-locale",QIcon(":/images/preferences-desktop-locale")).pixmap(48));

    QListWidgetItem *display = new QListWidgetItem(QObject::tr("Display"),ui->listWidget);
    display->setIcon(QIcon::fromTheme("preferences-desktop-display",QIcon(":/images/preferences-desktop-display")).pixmap(48));

    QListWidgetItem *notification = new QListWidgetItem(QObject::tr("Notifications"),ui->listWidget);
    notification->setIcon(QIcon::fromTheme("preferences-desktop-notification",QIcon(":/images/preferences-desktop-notification")).pixmap(48));

    QListWidgetItem *adhkar = new QListWidgetItem(QObject::tr("Adhkar"),ui->listWidget);
    adhkar->setIcon(QIcon::fromTheme("documentinfo",QIcon(":/images/preferences-desktop-notification")).pixmap(48));

    m_location=new LocationForm(this);
    m_notification = new NotificationForm(this);
    m_display = new DisplayForm(this);
    m_adhkar=new AdhkarForm(this);


    ui->vLayoutLocation->addWidget(m_location);
    ui->vLayoutDisplay->addWidget(m_display);
    ui->vLayoutNotifier->addWidget(m_notification);
ui->vLayoutAdhkar->addWidget(m_adhkar);
   ui->listWidget->setCurrentRow(0);
  #ifdef Q_OS_WIN32
   QSettings cg("elokab","adhan");
   cg.beginGroup("Adhan");
   ui->checkBoxAutoStart->setChecked(cg.value("AutoStart",false).toBool());
   cg.endGroup();
#else
   ui->checkBoxAutoStart->setVisible(false);
#endif
}

DialogConfig::~DialogConfig()
{
    delete m_location;
    delete m_notification ;
     delete m_display ;
    delete m_adhkar;
    delete ui;
}

void DialogConfig::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button)==QDialogButtonBox::Ok){

        m_location->saveSettings();
        m_notification->saveSettings();
        m_display->saveSettings();
m_adhkar->saveSettings();
        //QString path=QApplication::applicationDirPath()+"/elokab-adhan.exe";
        //path.replace("/","\\");
#ifdef Q_OS_WIN32
        QString filePath = QCoreApplication::applicationFilePath();

        //HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run

        QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run" , QSettings::NativeFormat);
        if(ui->checkBoxAutoStart->isChecked()){
            // settings.setValue("ElokabAdhan",path);
            settings.setValue("ElokabAdhan",  QDir::toNativeSeparators(filePath)) ;
        }else{
            //  settings.setValue("ElokabAdhan",QString());
            settings.remove("ElokabAdhan");

        }

        QSettings cg("elokab","adhan");
        cg.beginGroup("Adhan");
        cg.setValue("AutoStart",ui->checkBoxAutoStart->isChecked());
                cg.endGroup();
#endif
        emit   accepteChange();
        this->accept();
    }
}
