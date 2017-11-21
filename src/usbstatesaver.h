#ifndef USBSTATESAVER_H
#define USBSTATESAVER_H

#include <QObject>
#include <QFile>
#include "usbmodedcomm.h"

class UsbStateSaver : public QObject
{
    Q_OBJECT

public:
    static UsbStateSaver* Instance();
    UsbStateSaver(QObject *parent = 0);

public slots:
    bool forceUsbModeForIso();
    bool restoreUsbMode();

private:
    QString stateFilePath();
    QFile* m_savefile = Q_NULLPTR;
    UsbModedComm m_usbmcomm;

    static UsbStateSaver* s_instance;


signals:

};

#endif // USBSTATESAVER_H
