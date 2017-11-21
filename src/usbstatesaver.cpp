#include "usbstatesaver.h"
#include <QStandardPaths>
#include <QtDBus/QDBusInterface>
#include <QTextStream>
#include <QDir>

UsbStateSaver* UsbStateSaver::s_instance = nullptr;
UsbStateSaver* UsbStateSaver::Instance() {
    if (!s_instance)
        s_instance = new UsbStateSaver();
    return s_instance;
}

UsbStateSaver::UsbStateSaver(QObject *parent) : QObject(parent)
{
    this->m_savefile = new QFile(stateFilePath());
    QDir configDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    if (!configDir.exists())
        configDir.mkpath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
}

bool UsbStateSaver::forceUsbModeForIso()
{
    if (!m_savefile->open(QFile::ReadWrite | QFile::Truncate))
        return false;

    // Save state
    QTextStream tstream(m_savefile);
    tstream << m_usbmcomm.getActiveUsbMode();

    const bool ret = m_usbmcomm.setActiveMode(ISOMAN_USBMODED_GETCONFIG_VAL_CHARGE);
    m_savefile->close();
    return ret;
}

bool UsbStateSaver::restoreUsbMode()
{
    if (!m_savefile->open(QFile::ReadWrite | QFile::Truncate))
        return false;

    QTextStream tstream(m_savefile);
    const QString savedValue = tstream.readLine();

    const bool ret = m_usbmcomm.setActiveMode(savedValue);
    m_savefile->close();
    return ret;
}

QString UsbStateSaver::stateFilePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::DataLocation) +
            QStringLiteral("/usbState");
}
