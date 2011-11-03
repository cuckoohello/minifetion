#ifndef QFETIONCONTACTS_H
#define QFETIONCONTACTS_H

#include <QAbstractListModel>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDeclarativeParserStatus>
#include <QHash>
#include <openfetion.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct QFetionContact{
    QString  sid;
    QString userId;
    QString name;
    QString mobileno;
    int     groupId;
    QFetionContact(QString p_sid,QString p_userId,QString p_name,QString p_mobileno,int p_groupId)
    {
        sid = p_sid; userId = p_userId; name = p_name;
        mobileno = p_mobileno; groupId = p_groupId;
    }
};

struct  QFetionGroups{
    QString group;
    bool    show;
    QFetionGroups(QString p_group){
        group = p_group;
        show  = false;
    }
    QFetionGroups(){
        group = "NULL";
        show    = false;
    }
};


class QFetionContacts : public QAbstractListModel,public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)

    Q_PROPERTY(QString nickname READ getNickname)
    Q_PROPERTY(QString  password READ getPassword)
    Q_PROPERTY(QString  mobileno READ  getMobileNo)
    Q_PROPERTY(int total_contacts_count READ getTotalContactsCount NOTIFY contacts_Count_Changed);
    Q_PROPERTY(int presence_contacts_count READ getPresenceCount NOTIFY presence_Count_Changed);

public:
    explicit QFetionContacts(QObject *parent = 0);

    QString getNickname(){return nickname;}
    QString getPassword(){return password;}
    QString getMobileNo(){return mobileno;}
    int getTotalContactsCount(){return total_contacts;}
    int getPresenceCount() { return presence_count;}

    virtual void classBegin();
    virtual void componentComplete();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const ;

    Q_INVOKABLE void setGroupShow(int groupid){
        groupsList[groupid].show = !groupsList[groupid].show ;
        emit groupShowChanged(groupid);
    }
    Q_INVOKABLE QString getGroupName(int groupid){
        return groupsList[groupid].group;
    }

    Q_INVOKABLE bool getGroupVisible(int groupid){
        return groupsList[groupid].show;
    }
    Q_INVOKABLE bool sync_contacts(QString mobileno,QString password)
    {
        int ret = fx_login(mobileno.toUtf8().data(),password.toUtf8().data());
        emit sync_contacts_finished();
        return ret;
    }

    enum Roles {
        NameRole = Qt::DisplayRole,
        MobileRole,
        GroupRole,
        SectionRole,
        SidRole,
        UserIdRole,
        ImageRole,
        ShowRole
    };


protected:
    void fx_process_presence(User* user , const char* xml);
    void fx_process_notifications(User* user , const char* sipmsg);
    void* fx_listen_func(User* user);
    int fx_login(const char *mobileno, const char *password);
    void setTotalContacts(int num)
    {
        total_contacts = num;
        if(total_contacts != 0)
            emit contacts_Count_Changed();
    }
    void incPresenceCount()
    {
        presence_count++;
        emit presence_Count_Changed();
    }


signals:
    void groupShowChanged(int groupid);
    void sync_contacts_finished();
    void contacts_Count_Changed();
    void presence_Count_Changed();

public slots:
    void groupStateChanged(int groupid);

private:
    QList<QFetionContact>   contactsList;
  //   QSqlDatabase db;
  //   QSqlQuery query;
     QHash<int,QFetionGroups>   groupsList;
     QString    nickname,password,mobileno;

     User *user;

     int presence_count;
     fd_set  fd_read;
     int total_contacts;
};

#endif // QFETIONCONTACTS_H
