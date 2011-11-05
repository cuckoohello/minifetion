#ifndef QFETIONSTORESMS_H
#define QFETIONSTORESMS_H

#include <QAbstractListModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageManager>
#include <QMessageId>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDebug>


QTM_USE_NAMESPACE

struct QFetionMessage{
    QString message;
    QString date;
    int     status;
    int     id;
    QFetionMessage(int p_id,QString p_message,QString p_date, int p_status){
        id      = p_id;
        message = p_message;
        date    = p_date;
        status  = p_status;
    }
    void setStatus(int p_status)
    {
        status  = p_status;
    }
    int getID()
    {
        return  id;
    }
};

class QFetionStoreSMSThread;
class QFetionStoreSMS : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QFetionStoreSMS(QObject *parent = 0);
    Q_PROPERTY(QString uid READ getUid WRITE setUid NOTIFY uidChanged)
    Q_PROPERTY(int  limit READ getLimit WRITE setLimit NOTIFY limitChanged)


    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const ;

    QString getUid(){ return uid;}
    void setUid(QString p_uid){ uid = p_uid; emit uidChanged();}
    int getLimit() { return limit;}
    void setLimit(int p_limit){limit = p_limit; emit limitChanged();}

    Q_INVOKABLE int insertMessage(QString name,QString uid,QString message);
    Q_INVOKABLE void upDateMessage(QString id,QString status);

    enum Roles {
        DateRole = Qt::DisplayRole,
        StatusRole,
        MessageRole
    };


signals:
    void uidChanged();
    void initialSignal();
    void limitChanged();

public slots:
    void queryMessages();
    void	messageAdded ( const QMessageId & id, const QMessageManager::NotificationFilterIdSet & matchingFilterIds );

private:
    QList<QFetionMessage>   messageList;
    QMessageManager *messageManager;
     QSqlQuery  query;
     QString    uid;
     int        limit;
     QFetionStoreSMSThread *smsThread;

};

class QFetionStoreSMSThread:public QThread{
    Q_OBJECT
public:
    // NumThread(QObject *parent=0):QThread(parent){}
    QFetionStoreSMSThread(QFetionStoreSMS *p_storesms):QThread(0){storesms= p_storesms;}
    /*
    void run(){
        qDebug() << "begin messageManager";
        messageManager = new QMessageManager(this);
        connect(messageManager,SIGNAL(messageAdded(QMessageId,QMessageManager::NotificationFilterIdSet)),storesms,SLOT(messageAdded(QMessageId,QMessageManager::NotificationFilterIdSet)));
        messageManager->registerNotificationFilter(QMessageFilter());
        qDebug() <<"end messageMangager";
    };
    */

private:
    QFetionStoreSMS *storesms;
    QMessageManager *messageManager;
};

#endif // QFETIONSTORESMS_H
