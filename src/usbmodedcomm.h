#ifndef USBMODEDCOMM_H
#define USBMODEDCOMM_H

#include <QObject>
#include <QtDBus/QDBusInterface>

// Config/mode values interesting to ISODrive only
const QString ISOMAN_USBMODED_GETCONFIG_VAL_ASK     = QStringLiteral("ask");
const QString ISOMAN_USBMODED_GETCONFIG_VAL_CHARGE  = QStringLiteral("charging_only");

class UsbModedComm : public QObject
{
    Q_OBJECT
public:
    explicit UsbModedComm(QObject *parent = 0);

    // Here "Mode" doesn't correspond to UsbModed wording
    QString getActiveUsbMode();
    bool setActiveMode(const QString& config);

private:
    QDBusInterface* m_usbModedIface = Q_NULLPTR;

    bool isAsk();
};

#endif // USBMODEDCOMM_H
