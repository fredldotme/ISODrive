#include "filemanager.h"

FileManager::FileManager(QObject *parent) :
    QObject(parent)
{
}

QStringList FileManager::getISOFiles()
{
    QStringList ret;
    QDir downloadDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));

    QFileInfoList fileList = downloadDir.entryInfoList();
    for(int i = 0; i < fileList.count(); i++) {
        if (fileList[i].fileName().endsWith(".iso")) {
            ret.append(fileList[i].fileName());
        }
    }
    return ret;
}

