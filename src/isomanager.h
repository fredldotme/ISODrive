#ifndef ISOMANAGER_H
#define ISOMANAGER_H

#include <QObject>
#include <QFile>

class ISOManager : public QObject
{
    Q_OBJECT
public:
    ISOManager(QObject *parent = 0);
    Q_PROPERTY(QString selectedISO READ getSelectedISO NOTIFY selectedISOChanged)

    Q_INVOKABLE void enableISO(QString fileName);
    Q_INVOKABLE bool isEnabledISO(QString fileName);
    Q_INVOKABLE void resetISO();

private:
    bool enabled();
    void setEnabled(bool enabled);
    QString getSelectedISO();
    QString getSelectedISOPath();

signals:
    void selectedISOChanged();
    void selectionFailed();

public slots:

};

#endif // ISOMANAGER_H
