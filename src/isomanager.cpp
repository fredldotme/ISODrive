#include "isomanager.h"
#include "filemanager.h"

ISOManager::ISOManager(QObject *parent) :
    QObject(parent)
{
}

void ISOManager::enableISO(QString fileName)
{
    if(enabled())
        setEnabled(false);

    QString absolutePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + fileName;

    QFile lunFileSysfs(SYSFS_LUN_FILE);
    lunFileSysfs.open(QFile::ReadWrite);
    if(!fileName.isEmpty()) {
        lunFileSysfs.write(absolutePath.toUtf8().data());
    } else {
        lunFileSysfs.write(" ");
    }
    lunFileSysfs.flush();
    lunFileSysfs.close();

    emit selectedISOChanged();

    lunFileSysfs.open(QFile::ReadWrite);
    if(!fileName.isEmpty() && !QString(lunFileSysfs.readAll()).startsWith(absolutePath)) {
        emit selectionFailed();
    }
    lunFileSysfs.close();

    setEnabled(true);
}

void ISOManager::resetISO()
{
    enableISO("");
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

QString ISOManager::getSelectedISO()
{
    QFile lunFileSysfs(SYSFS_LUN_FILE);
    lunFileSysfs.open(QFile::ReadWrite);
    QString content(lunFileSysfs.readAll());
    QString fileName = content.mid(content.lastIndexOf("/") + 1);
    return fileName.isEmpty() ? "none" : fileName;
}
