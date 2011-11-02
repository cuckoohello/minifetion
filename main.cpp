#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include <qdeclarative.h>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include "qfetionsms.h"
#include "qfetioncontacts.h"
#include "qfetionstoresms.h"
//#include "qfetionnetwork.h"

#include <QSqlDatabase>

Q_DECL_EXPORT int main(int argc, char *argv[])
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/user/.fetion/data.db");
    if (!db.open()) {
        qDebug()  <<"Sql open error!\n";
    }

    //  qmlRegisterType<QFetionSMS>("com.cuckoo.fetion", 1, 0, "FetionSMSModel");
      qmlRegisterType<QFetionContacts>("com.cuckoo.fetion", 1, 0, "FetionContacts");
      qmlRegisterType<QFetionStoreSMS>("com.cuckoo.fetion",1,0,"FetionSMSModel");
   //   qmlRegisterType<QFetionNetwork>("com.cuckoo.fetion",1,0,"FetionNetwork");
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    viewer->engine()->setOfflineStoragePath("/home/user/.local/share//data///QML/OfflineStorage");
 //   qDebug() << viewer->engine()->offlineStoragePath();

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/fetion/smsui.qml"));
    viewer->showExpanded();

    return app->exec();
}
