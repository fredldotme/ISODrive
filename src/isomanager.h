#ifndef ISOMANAGER_H
#define ISOMANAGER_H

#define SYSFS_ENABLE "/sys/devices/virtual/android_usb/android0/enable"
#define SYSFS_LUN_FILE "/sys/devices/virtual/android_usb/android0/f_mass_storage/lun/file"

#include <QObject>
#include <QFile>

class ISOManager : public QObject
{
    Q_OBJECT
public:
    ISOManager(QObject *parent = 0);
    Q_PROPERTY(QString selectedISO READ getSelectedISO NOTIFY selectedISOChanged)

    Q_INVOKABLE void enableISO(QString fileName);
    Q_INVOKABLE void resetISO();

private:
    bool enabled();
    void setEnabled(bool enabled);
    QString getSelectedISO();

signals:
    void selectedISOChanged();
    void selectionFailed();

public slots:

};

#endif // ISOMANAGER_H
