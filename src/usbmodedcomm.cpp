#include "usbmodedcomm.h"
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

// DBus connection properties
const QString ISOMAN_USBMODED_SERVICE = QStringLiteral("com.meego.usb_moded");
const QString ISOMAN_USBMODED_PATH    = QStringLiteral("/com/meego/usb_moded");
const QString ISOMAN_USBMODED_IFACE   = QStringLiteral("com.meego.usb_moded");

// Persistent
const QString ISOMAN_USBMODED_METHOD_GETCONFIG   = QStringLiteral("get_config");
const QString ISOMAN_USBMODED_METHOD_SETCONFIG   = QStringLiteral("set_config");

// Temporary mode request ("ask") dialog
const QString ISOMAN_USBMODED_METHOD_SETMODE     = QStringLiteral("set_mode");
const QString ISOMAN_USBMODED_METHOD_MODEREQUEST = QStringLiteral("mode_request");

UsbModedComm::UsbModedComm(QObject *parent) : QObject(parent)
{
    m_usbModedIface = new QDBusInterface(ISOMAN_USBMODED_SERVICE,
                                         ISOMAN_USBMODED_PATH,
                                         ISOMAN_USBMODED_IFACE,
                                         QDBusConnection::systemBus(),
                                         this);
}

QString UsbModedComm::getActiveUsbMode()
{
    const QString& getMethod = isAsk() ? ISOMAN_USBMODED_METHOD_MODEREQUEST :
                                         ISOMAN_USBMODED_METHOD_GETCONFIG;
    const QDBusReply<QString> active_mode = m_usbModedIface->call(getMethod);
    return active_mode.value();
}

bool UsbModedComm::setActiveMode(const QString &config)
{
    const QString& setMethod = isAsk() ? ISOMAN_USBMODED_METHOD_SETMODE :
                                         ISOMAN_USBMODED_METHOD_SETCONFIG;

    QDBusReply<QString> reply = m_usbModedIface->call(setMethod, QVariant::fromValue<QString>(config));
    return (config.compare(reply.value(), Qt::CaseInsensitive));
}

bool UsbModedComm::isAsk()
{
    const QDBusReply<QString> active_mode =
            m_usbModedIface->call(ISOMAN_USBMODED_METHOD_GETCONFIG);
    return (ISOMAN_USBMODED_GETCONFIG_VAL_ASK.compare(active_mode.value()), Qt::CaseInsensitive);
}
