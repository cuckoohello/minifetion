#include "qfetionsms.h"

QFetionSMS::QFetionSMS(QObject *parent) :
    QAbstractListModel(parent)
{
    qDebug("initial QfetionSMS!\n");
    isInitialed = false;
}

void    QFetionSMS::initial()
{
    if (!isInitialed)
    {
        messageManager = new QMessageManager(this);
        connect(messageManager,SIGNAL(messageAdded(QMessageId,QMessageManager::NotificationFilterIdSet)),this,SLOT(messageAdded(QMessageId,QMessageManager::NotificationFilterIdSet)));
        connect(messageManager,SIGNAL(messageRemoved(QMessageId,QMessageManager::NotificationFilterIdSet)),this,SLOT(messageRemoved(QMessageId,QMessageManager::NotificationFilterIdSet)));
        connect(messageManager,SIGNAL(messageUpdated(QMessageId,QMessageManager::NotificationFilterIdSet)),this,SLOT(messageUpdated(QMessageId,QMessageManager::NotificationFilterIdSet)));
        connect(this,SIGNAL(cellphoneChanged()),this,SLOT(getNewMessageList()));

        QHash<int, QByteArray> roleNames;
        roleNames[SubjectRole] = "subject";
        roleNames[SenderRole] = "sender";
        roleNames[ToRole] = "to";
        roleNames[SizeRole] = "size";
        roleNames[TypeRole] = "type";
        roleNames[DateRole] = "date";
        roleNames[ReceivedDateRole] = "receivedDate";
        roleNames[BodyRole] = "body";
        roleNames[PriorityRole] = "priority";
        roleNames[InboxRole] = "isInbox";
        roleNames[ReadyRole] = "ready";
        setRoleNames(roleNames);
        filterid = messageManager->registerNotificationFilter(QMessageFilter());

        isInitialed = true;
    }
}


void	QFetionSMS::messageAdded ( const QMessageId & id, const QMessageManager::NotificationFilterIdSet & matchingFilterIds )
{
    QMessage message = messageManager->message(id);
    bool found = false;
    if (!message.from().addressee().contains(cellphone))
    {
        foreach(const QMessageAddress &address,message.to())
        {
            if(address.addressee().contains(cellphone))
            {
                found = true;
                break;
            }
        }
    }else
        found = true;

    if (found)
    {
        beginInsertRows(QModelIndex(),ids.count(),ids.count()+1);
        ids.append(id);
        endInsertRows();
        usleep(200000);
        emit countChanged();
    }

}

void	QFetionSMS::messageRemoved ( const QMessageId & id, const QMessageManager::NotificationFilterIdSet & matchingFilterIds )
{
    qDebug("Message Removed!\n");

}

void	QFetionSMS::messageUpdated ( const QMessageId & id, const QMessageManager::NotificationFilterIdSet & matchingFilterIds )
{
    qDebug("MessageUpdated!\n");

}


 void QFetionSMS::classBegin()
{
    qDebug("Class begin!\n");
}

 void QFetionSMS::componentComplete()
{
    qDebug("Class completed!\n");
}

 int QFetionSMS::rowCount(const QModelIndex &parent) const
{
   //  qDebug("call rowCount");
    return ids.count();
}

 QVariant QFetionSMS::data(const QModelIndex &index, int role ) const {

     if(isInitialed)
     {
         //  qDebug("%d\n",index.row());
         if (!index.isValid())
             return QString();
         if (index.row()>ids.count())
             return QString();
         QMessage message = QtMobility::QMessageManager().message(ids.at(index.row()));
         switch(role)
         {
         case SubjectRole:
             return message.subject();
             break;
         case SizeRole:
             return message.size();
             break;
         case TypeRole:
             return message.type();
             break;
         case SenderRole:
             return message.from().addressee();
             break;
         case ToRole: {
             QStringList to;
             foreach (QMessageAddress addr, message.to())
                 to += addr.addressee();
             return to;
         }
             break;
         case DateRole:
             if (message.date().toLocalTime().daysTo(QDateTime::currentDateTime()) == 0)
                 return message.date().toLocalTime().toString("h:mm AP");
             else
                 return message.date().toLocalTime().toString("yy-MM-dd|h:mm AP");
             break;
         case ReceivedDateRole:
             if (message.date().toLocalTime().daysTo(QDateTime::currentDateTime()) == 0)
                 return message.date().toLocalTime().toString("h:mm AP");
             else
                 return message.date().toLocalTime().toString("yy-MM-dd|h:mm AP");
             break;
         case BodyRole:
             return message.textContent();
             break;
         case PriorityRole:
             return message.priority();
             break;
         case InboxRole:
             return message.standardFolder()==QMessage::InboxFolder;
             break;
         case ReadyRole:
             return true;
             break;
         default:
             break;
         }
         return QString();
     }else
         return QString();
}

void QFetionSMS::getNewMessageList()
{
    if(isInitialed)
    {
        beginResetModel();
        ids.clear();
        ids = messageManager->queryMessages(QMessageFilter::bySender(cellphone)| QMessageFilter::byRecipients(cellphone),
                                            QMessageSortOrder::byTimeStamp(Qt::AscendingOrder));
        endResetModel();

        emit countChanged();
    }

}

