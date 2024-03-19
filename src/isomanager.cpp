#include "isomanager.h"
#include "filemanager.h"
#include "usbstatesaver.h"

#include <QUrl>

#define SYSFS_ENABLE_CLASSIC "/sys/devices/virtual/android_usb/android0/enable"
#define SYSFS_LUN_FILE_CLASSIC "/sys/devices/virtual/android_usb/android0/f_mass_storage/lun/file"
#define SYSFS_LUN_FILE_XPERIA10_2 "/sys/kernel/config/usb_gadget/g1/functions/mass_storage.usb0/lun.0/file"
#define SYSFS_LUN_MODE_CDROM_XPERIA10_2 "/sys/kernel/config/usb_gadget/g1/functions/mass_storage.usb0/lun.0/cdrom"
#define SYSFS_LUN_MODE_RO_XPERIA10_2 "/sys/kernel/config/usb_gadget/g1/functions/mass_storage.usb0/lun.0/ro"

ISOManager::ISOManager(QObject *parent) :
    QObject(parent)
{
    QString op_lunFileSysfs = SYSFS_LUN_FILE_CLASSIC;
}

void ISOManager::enableISO_classic(QString fileName)
{
    if(enabled())
        setEnabled(false);

    const QString absolutePath = fileName.mid(7);
    QFile lunFileSysfs(SYSFS_LUN_FILE_CLASSIC);
    lunFileSysfs.open(QFile::ReadWrite | QFile::Truncate);
    if(!fileName.isEmpty()) {
        lunFileSysfs.write(absolutePath.toUtf8().data());
    } else {
        lunFileSysfs.reset();
        lunFileSysfs.write(QByteArrayLiteral("\n"));
    }
    lunFileSysfs.flush();
    lunFileSysfs.close();

    emit selectedISOChanged();

    if(!lunFileSysfs.open(QFile::ReadWrite) ||
            (!fileName.isEmpty() && !QString(lunFileSysfs.readAll()).startsWith(absolutePath))) {
        emit selectionFailed();
    }
    lunFileSysfs.close();

    setEnabled(true);
}

void ISOManager::enableISO_xperia10_2(QString fileName)
{
    const QString absolutePath = fileName.mid(7);
    //
    QFile cdromlunFileSysfs(SYSFS_LUN_MODE_CDROM_XPERIA10_2);
    QFile rolunFileSysfs(SYSFS_LUN_MODE_RO_XPERIA10_2);
    cdromlunFileSysfs.open(QFile::ReadWrite | QFile::Truncate);
    cdromlunFileSysfs.reset();
    cdromlunFileSysfs.write(QByteArrayLiteral("1"));
    cdromlunFileSysfs.flush();
    cdromlunFileSysfs.close();

    rolunFileSysfs.open(QFile::ReadWrite | QFile::Truncate);
    rolunFileSysfs.reset();
    rolunFileSysfs.write(QByteArrayLiteral("1"));
    rolunFileSysfs.flush();
    rolunFileSysfs.close();
    //
    QFile lunFileSysfs(SYSFS_LUN_FILE_XPERIA10_2);
    lunFileSysfs.open(QFile::ReadWrite | QFile::Truncate);
    if(!fileName.isEmpty()) {
        lunFileSysfs.write(absolutePath.toUtf8().data());
    } else {
        lunFileSysfs.reset();
        lunFileSysfs.write(QByteArrayLiteral("\n"));
    }
    lunFileSysfs.flush();
    lunFileSysfs.close();

    emit selectedISOChanged();

    if(!lunFileSysfs.open(QFile::ReadWrite) ||
            (!fileName.isEmpty() && !QString(lunFileSysfs.readAll()).startsWith(absolutePath))) {
        emit selectionFailed();
    }
    lunFileSysfs.close();
}

void ISOManager::enableISO(QString fileName)
{
    const QString absolutePath = fileName.mid(7);

    QFile lunFileSysfs(SYSFS_LUN_FILE_CLASSIC);
    if (lunFileSysfs.exists()) {
        op_lunFileSysfs=SYSFS_LUN_FILE_CLASSIC;
        enableISO_classic(fileName);
    } else {
        QFile lunFileSysfs(SYSFS_LUN_FILE_XPERIA10_2);
        if (lunFileSysfs.exists()) {
            op_lunFileSysfs=SYSFS_LUN_FILE_XPERIA10_2;
            enableISO_xperia10_2(fileName);
        }
    }
}

bool ISOManager::isEnabledISO(QString fileName)
{
    const QString absolutePath = fileName.mid(7);
    return (absolutePath.compare(getSelectedISOPath(), Qt::CaseInsensitive) == 0);
}

void ISOManager::resetISO()
{
    enableISO("");
    //UsbStateSaver::Instance()->restoreUsbMode();
}


bool ISOManager::enabled()
{
    QFile enableSysfs(SYSFS_ENABLE_CLASSIC);
    enableSysfs.open(QFile::ReadOnly);
    QString content(enableSysfs.readAll());
    bool ret = content.startsWith("1");
    enableSysfs.close();
    return ret;
}

/*
bool ISOManager::enabled()
{
    QFile enableSysfs(mylunFileSysfs);
    enableSysfs.open(QFile::ReadOnly);
    QString content(enableSysfs.readAll());
    bool ret = content.startsWith("/");
    enableSysfs.close();
    return ret;
}
*/
void ISOManager::setEnabled(bool enabled)
{
    QFile enableSysfs(SYSFS_ENABLE_CLASSIC);
    enableSysfs.open(QFile::ReadWrite);
    enableSysfs.write(enabled ? "1" : "0", 1);
    enableSysfs.flush();
    enableSysfs.close();
}

QString ISOManager::getSelectedISOPath()
{
    QFile lunFileSysfs(op_lunFileSysfs);
    lunFileSysfs.open(QFile::ReadWrite);
    QString content(lunFileSysfs.readAll());
    QString fileName = content;
    fileName = fileName.replace("\n", "");
    return fileName.trimmed();
}

QString ISOManager::getSelectedISO()
{
    const QString content = getSelectedISOPath();
    QString fileName = content.mid(content.lastIndexOf("/") + 1);
    fileName = fileName.replace("\n", "");
    return fileName.trimmed();
}
