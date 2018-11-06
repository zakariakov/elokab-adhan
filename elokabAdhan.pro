TEMPLATE    = subdirs
SUBDIRS	    = elokab-adhan

 MKDIR = mkdir -p /usr/share/elokab/elokab-adhan/data
 MKDIR = mkdir -p /usr/share/elokab/translations
 MKDIR = mkdir -p /usr/share/elokab/translations
 MKDIR = mkdir -p /usr/share/icons/hicolor/scalable/apps/

 appdesktop.files =usr/share/applications/elokab-adhan.desktop
 appdesktop.path=/usr/share/applications/

 data.files =usr/share/elokab/elokab-adhan/data/*
 data.path=/usr/share/elokab/elokab-adhan/data

 translations.files =usr/share/elokab/translations/*
 translations.path=/usr/share/elokab/translations

 scalable.files=usr/share/icons/hicolor/scalable/apps/elokab-adhan.svg
 scalable.path =/usr/share/icons/hicolor/scalable/apps/


 INSTALLS +=        data\
                translations\
                appdesktop\
scalable
