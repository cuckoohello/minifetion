#include <QtGui/QApplication>
#include <qdeclarative.h>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include "qfetionsms.h"
#include "qfetioncontacts.h"
#include "qfetionstoresms.h"
#include "fpsdeclarativeview.h"
#include <QDir>
#include <QFile>
//#include "qfetionnetwork.h"

#include <QSqlDatabase>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/user/.fetion/data.db");

    QFile file("/home/user/.fetion/data.db");
    if (!file.exists())
    {
        QDir dir("/home/user/.fetion");
        if (!dir.exists())
            dir.mkpath("/home/user/.fetion");
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    if (!db.open()) {
        qDebug()  <<"Sql open error!\n";
    }

    //  qmlRegisterType<QFetionSMS>("com.cuckoo.fetion", 1, 0, "FetionSMSModel");
      qmlRegisterType<QFetionContacts>("com.cuckoo.fetion", 1, 0, "FetionContacts");
      qmlRegisterType<QFetionStoreSMS>("com.cuckoo.fetion",1,0,"FetionSMSModel");
      QApplication app(argc, argv);
   //   qmlRegisterType<QFetionNetwork>("com.cuckoo.fetion",1,0,"FetionNetwork");

      QDir::setCurrent(app.applicationDirPath());
      FPSDeclarativeView window;
      window.setSource(QUrl("qrc:/smsui.qml"));

  #ifndef Q_WS_MACX
      window.showFullScreen();
  #else
      window.show();
  #endif

  #ifndef __arm__
        window.resize(window.initialSize().width(), window.initialSize().height());
  #endif


    return app.exec();
}
