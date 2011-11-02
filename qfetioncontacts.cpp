#include "qfetioncontacts.h"
#include <QFile>

QFetionContacts::QFetionContacts(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roleNames;
    roleNames[NameRole] = "name";
    roleNames[MobileRole] = "phone";
    roleNames[GroupRole] = "group";
    roleNames[SectionRole] = "section";
    roleNames[SidRole] = "sid";
    roleNames[UserIdRole] = "userid";
    roleNames[ImageRole] = "image";
    roleNames[ShowRole] = "show";
    setRoleNames(roleNames);

    QSqlQuery query;
    QSqlQuery queryContacts;

    if(!query.exec("select mobileno,password,nickname from user"))
        qDebug() << "Get user information error!";
    else
    {
        query.next();
        nickname = query.value(2).toString();
        password = query.value(1).toString();
        mobileno = query.value(0).toString();
    }

    int groupId;
    if(!query.exec("select * from groups order by groupid"))
        qDebug() <<"Select from groups error!";

    while(query.next())
    {
        groupId = query.value(0).toInt();

        groupsList[groupId] = QFetionGroups(query.value(1).toString());
        QString queryString("select sId,userId,localname,nickname,mobileno from contacts_2_2_0 where groupids='");
        if(!queryContacts.exec(queryString.append(query.value(0).toString()).append("' order by localname,nickname")))
            qDebug() <<"select contacts from " << groupsList[groupId].group<<" error ";

        while(queryContacts.next())
        {
            if (queryContacts.value(4).toString()!="")
              contactsList.append(QFetionContact(queryContacts.value(0).toString(),queryContacts.value(1).toString(),
                                               (queryContacts.value(2).toString()=="") ? queryContacts.value(3).toString() :
                                                                                         queryContacts.value(2).toString(),
                                               queryContacts.value(4).toString(),groupId));
        }
    }

    connect(this,SIGNAL(groupShowChanged(int)),this,SLOT(groupStateChanged(int)));
}

void QFetionContacts::classBegin(){
    qDebug() <<"QFetionContacts Class begin";
}

void QFetionContacts::componentComplete(){
    qDebug() <<"QFetionContacts completed!";
}

void QFetionContacts::groupStateChanged(int groupid){
    int from = -1,end = -1,count = contactsList.count();
    for (int i= 0;i <count;i++)
    {
        if(contactsList.at(i).groupId == groupid)
        {
            if (from == -1)
                from = i;
            end = i;
        }else if (from != -1)
            break;
    }
    QModelIndex idx_from = index(from, 0);
    QModelIndex idx_end = index(end,0);
    emit dataChanged(idx_from,idx_end);
}


QVariant QFetionContacts::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QString();
    if (index.row()>rowCount())
        return QString();
    QFetionContact  contact = contactsList.at(index.row());

    QString file("/home/user/.fetion/icons/");
    switch(role)
    {
    case NameRole:
        return contact.name;
        break;
    case MobileRole:
        return contact.mobileno;
        break;
    case GroupRole:
        return contact.groupId;
        break;
    case SectionRole:
        return groupsList[contact.groupId].group;
        break;
    case SidRole:
        return  contact.sid;
        break;
    case UserIdRole:
        return  contact.userId;
        break;
    case ImageRole:
        file.append(contact.sid).append(".jpg");
        if (QFile(file).exists())
            return file;
        else
            return QString("images/default.jpg");
        break;
    case ShowRole:
        return  groupsList[contact.groupId].show;
        break;
    default:
        break;
    }
    return QString();
}


int QFetionContacts::rowCount(const QModelIndex &parent ) const{
    return contactsList.count();
}

