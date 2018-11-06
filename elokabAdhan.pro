TEMPLATE    = subdirs
SUBDIRS	    = elokab-adhan



 appdesktop.files =usr/share/applications/elokab-adhan.desktop
 appdesktop.path=/usr/share/applications/
 data.files =usr/share/elokab/elokab-adhan*
 data.path=/usr/share/elokab/
 translations.files =usr/share/elokab/translations/*
 translations.path=/usr/share/elokab/

 scalable.files=../usr/share/icons/hicolor/scalable/apps/elokab-adhan.svg
 scalable.path =/usr/share/icons/hicolor/scalable/apps/


 INSTALLS +=        data\
                translations\
                appdesktop\
scalable
