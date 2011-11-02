#ifndef QFETIONCONTACTS_H
#define QFETIONCONTACTS_H

#include <QAbstractListModel>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDeclarativeParserStatus>
#include <QHash>

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

public:
    explicit QFetionContacts(QObject *parent = 0);

    QString getNickname(){return nickname;}
    QString getPassword(){return password;}
    QString getMobileNo(){return mobileno;}

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

signals:
    void groupShowChanged(int groupid);

public slots:
    void groupStateChanged(int groupid);

private:
    QList<QFetionContact>   contactsList;
  //   QSqlDatabase db;
  //   QSqlQuery query;
     QHash<int,QFetionGroups>   groupsList;
     QString    nickname,password,mobileno;
};

#endif // QFETIONCONTACTS_H
