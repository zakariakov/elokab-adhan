#ifndef NOTIFICATIONUTILL_H
#define NOTIFICATIONUTILL_H
#include <QDBusInterface>
#include <QDebug>

/**
 * @brief The NotificationUtill class
 */
class NotificationUtill
{
public:
    NotificationUtill();
public:

    static void Notify(const QString &app_name, const QString &app_icon,
                       const QString &summary, const QString &body,
                           int expire_timeout)
    {
        QDBusInterface dbus("org.freedesktop.Notifications",
                            "/org/freedesktop/Notifications",
                            "org.freedesktop.Notifications");

        if (!dbusFreedesktopIsValid()) {
            qDebug() << "QDBusInterface is not valid!";
            return ;
        }

        QList<QVariant> args;
        args.append(app_name);       // Application Name
        args.append(0123U);         // Replaces ID (0U)
        args.append(app_icon);     // Notification Icon
        args.append( summary);       // Summary
        args.append(body);          // Body
        args.append(QStringList()); // Actions
        args.append(QVariantMap());
        args.append(expire_timeout);
        dbus.callWithArgumentList(QDBus::NoBlock, "Notify", args);
    }

    static bool dbusFreedesktopIsValid()
    {
        QDBusInterface dbus("org.freedesktop.Notifications",
                            "/org/freedesktop/Notifications",
                            "org.freedesktop.Notifications");

        if (!dbus.isValid()) {
            qDebug() << "QDBusInterface is not valid!";
            return false;
        }
        return true;
    }
};

#endif // NOTIFICATIONUTILL_H
