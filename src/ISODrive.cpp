#include <QtQuick>
#include <sailfishapp.h>
#include "filemanager.h"
#include "isomanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setSetuidAllowed(true);
    QGuiApplication *app = SailfishApp::application(argc, argv);
    QQuickView *view = SailfishApp::createView();

    qmlRegisterType<FileManager>("FileManager", 1, 0, "FileManager");
    qmlRegisterType<ISOManager>("ISOManager", 1, 0, "ISOManager");


    view->rootContext()->setContextProperty("fileManager", new FileManager());
    view->rootContext()->setContextProperty("isoManager", new ISOManager());
    view->setSource(SailfishApp::pathTo("qml/ISODrive.qml"));
    view->showFullScreen();

    return app->exec();
}

