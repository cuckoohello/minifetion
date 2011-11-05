#include "qfetionstoresms.h"
#include <QDateTime>
#include <QDebug>


QFetionStoreSMS::QFetionStoreSMS(QObject *parent) :
    QAbstractListModel(parent)
{
    isInitialed = false;
    smsThread = new QFetionStoreSMSThread(this);
    connect(this,SIGNAL(doThread(QFetionStoreSMS::DoThread)),smsThread,SLOT(doThread(QFetionStoreSMS::DoThread)));
    smsThread->start();
  //  usleep(1000);
  //  emit doThread(QFetionStoreSMS::InitialClass);

}

void QFetionStoreSMS::initial()
{
    emit doThread(QFetionStoreSMS::InitialClass);
}

void QFetionStoreSMS::initialClass()
{
    if(!isInitialed)
    {
        if(!query.exec("create table  IF NOT EXISTS  history (id INTEGER PRIMARY KEY AUTOINCREMENT,name TEXT,userid TEXT,message TEXT,updatetime TEXT,issend INTEGER)"))
            qDebug() << "create history table error!";

        QHash<int, QByteArray> roleNames;
        limit   = 20;

        roleNames[DateRole] = "date";
        roleNames[StatusRole] = "status";
        roleNames[MessageRole] = "body";
        setRoleNames(roleNames);

        connect(this,SIGNAL(uidChanged()),this,SLOT(queryMessages()));
        messageManager = new QMessageManager(this);
        connect(messageManager,SIGNAL(messageAdded(QMessageId,QMessageManager::NotificationFilterIdSet)),this,SLOT(messageAdded(QMessageId,QMessageManager::NotificationFilterIdSet)));
        messageManager->registerNotificationFilter(QMessageFilter());
        isInitialed = true;
    }
}


int QFetionStoreSMS::rowCount(const QModelIndex &parent ) const {
    return messageList.count();
}


QVariant QFetionStoreSMS::data(const QModelIndex &index, int role ) const {
    if (!index.isValid())
        return QString();
    if (index.row()>messageList.count())
        return QString();
    QFetionMessage  message = messageList.at(index.row());
    QDateTime   dateTime = QDateTime::fromString(message.date,"yyyy-MM-dd hh:mm:ss");

    QString result;

    switch(role)
    {
    case DateRole:
        if(message.status == 0)
        {}
        else if(message.status == 1)
            result = "Sent|";
        else if(message.status == 2)
            result = "Pending|";
        else
            result = "Error|";
        if (dateTime.daysTo(QDateTime::currentDateTime()) == 0)
            return result.append(dateTime.toString("h:mm AP"));
        else
            return result.append(dateTime.toString("yy-MM-dd|h:mm AP"));

        break;
    case StatusRole:
        return  message.status;
        break;
    case MessageRole:
        return message.message;
        break;
    default:
        break;
    }
    return QString();

}


void    QFetionStoreSMS::queryMessages(){
    QString messageQuery("select id,message,updatetime,issend  from history where userid='");
    messageQuery.append(uid).append("' order by updatetime desc limit ").append(QString::number(limit));
    qDebug() << messageQuery;
    if(!query.exec(messageQuery))
        qDebug() <<"message query "<<messageQuery<<" error";
    beginResetModel();
    messageList.clear();
    while(query.next())
    {
        messageList.insert(0,QFetionMessage(query.value(0).toInt(),query.value(1).toString(),query.value(2).toString(),query.value(3).toInt()));
    }
    endResetModel();
}

int  QFetionStoreSMS::insertMessage(QString name, QString uid, QString message)
{
    QString sql("insert into history values (NULL,'");
    sql.append(name).append("','").append(uid).append("','").append(message).append("','");
    sql.append(QDateTime::currentDateTime().toLocalTime().toString("yyyy-MM-dd hh:mm:ss")).append("',2);");

    if(!query.exec(sql))
    {
        qDebug() <<"insert message error!"<<sql;
    }

    emit uidChanged();

    if(!query.exec("select id from history where issend=2 order by id desc limit 1;"))
        qDebug() <<" get message id error";
    if(query.next())
    {
        return query.value(0).toInt();
    }else
        qDebug() <<"Error message id return";
    return -1;
}

void  QFetionStoreSMS::upDateMessage(QString id,QString status)
{
    QString sql("update history set issend=");
    sql.append(status).append(" where id=").append(id);
    qDebug() <<"update message "<<id<<" status "<<status;
    if(!query.exec(sql))
        qDebug() <<"update message "<<id <<" status error!";

    emit uidChanged();
}


void	QFetionStoreSMS::messageAdded ( const QMessageId & id, const QMessageManager::NotificationFilterIdSet & matchingFilterIds )
{

    QMessage message = messageManager->message(id);
    QString from = message.from().addressee();

    if (from.startsWith("12520"))
        from = from.remove(0,5);
    else if (from.startsWith("+86"))
        from = from.remove(0,3);

    QString sql("select userId,localname,nickname from contacts_2_2_0 where mobileno='");
    sql.append(from).append("'");

    if(!query.exec(sql))
        qDebug() << "Query contact "<<from<<" error!";

    if(query.next())
    {
        QString   userId = query.value(0).toString();
        QString   localname = query.value(1).toString();
        QString    nickname = query.value(2).toString();
        QString     name = (localname=="") ? nickname : localname;

        QString sql("insert into history values (NULL,'");
        sql.append(name).append("','").append(userId).append("','").append(message.textContent()).append("','");
        sql.append(message.receivedDate().toLocalTime().toString("yyyy-MM-dd hh:mm:ss")).append("',0);");
        if(!query.exec(sql))
            qDebug() << "insert message error "<<sql;
        if (userId == uid)
            emit uidChanged();
    }
}

