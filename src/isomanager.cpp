#include "isomanager.h"
#include "filemanager.h"
#include "usbstatesaver.h"

#include <QUrl>

#define SYSFS_ENABLE "/sys/devices/virtual/android_usb/android0/enable"
#define SYSFS_LUN_FILE "/sys/devices/virtual/android_usb/android0/f_mass_storage/lun/file"

ISOManager::ISOManager(QObject *parent) :
    QObject(parent)
{
}

void ISOManager::enableISO(QString fileName)
{
    /*if (!UsbStateSaver::Instance()->forceUsbModeForIso())
        return;*/

    if(enabled())
        setEnabled(false);

    const QString absolutePath = fileName.mid(7);

    QFile lunFileSysfs(SYSFS_LUN_FILE);
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
    QFile enableSysfs(SYSFS_ENABLE);
    enableSysfs.open(QFile::ReadOnly);
    QString content(enableSysfs.readAll());
    bool ret = content.startsWith("1");
    enableSysfs.close();
    return ret;
}

void ISOManager::setEnabled(bool enabled)
{
    QFile enableSysfs(SYSFS_ENABLE);
    enableSysfs.open(QFile::ReadWrite);
    enableSysfs.write(enabled ? "1" : "0", 1);
    enableSysfs.flush();
    enableSysfs.close();
}

QString ISOManager::getSelectedISOPath()
{
    QFile lunFileSysfs(SYSFS_LUN_FILE);
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
