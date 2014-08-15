#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#define DOWNLOADS_DIRECTORY "/home/nemo/Downloads/"

#include <QObject>
#include <QDebug>
#include <QDir>

class FileManager : public QObject
{
    Q_OBJECT
public:
    FileManager(QObject *parent = 0);
    Q_INVOKABLE QStringList getISOFiles();

signals:

public slots:

};

#endif // FILEMANAGER_H
