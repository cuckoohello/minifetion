#ifndef QFETIONSMS_H
#define QFETIONSMS_H

#include <QDeclarativeParserStatus>
#include <QAbstractListModel>
#include <QMessage>
#include <QDebug>
#include <QMessageId>
#include <QMessageManager>

QTM_USE_NAMESPACE

class QFetionSMS : public QAbstractListModel,public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QDeclarativeParserStatus)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    Q_PROPERTY(QString cellphone READ getCellphone WRITE setCellphone NOTIFY cellphoneChanged)

    public:
    explicit QFetionSMS(QObject *parent = 0);

    virtual void classBegin();
    virtual void componentComplete();

    Q_INVOKABLE void  initial();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const ;

    int count() const {return ids.count();};
    int limit() const { return m_limit;};
    QString getCellphone() const {return cellphone;}

    void setCellphone(QString cell){cellphone = cell;emit cellphoneChanged();}

    void setLimit(const int limit){ m_limit = limit;};

    enum Roles {
        SubjectRole = Qt::DisplayRole,
        TypeRole,
        SizeRole,
        SenderRole,
        ToRole,
        DateRole,
        ReceivedDateRole,
        BodyRole,
        PriorityRole,
        InboxRole,
        ReadyRole
    };


    signals:
    void limitChanged();
    void countChanged();
    void cellphoneChanged();
     void itemsRemoved(int index, int count);

    public slots:
    void	messageAdded ( const QMessageId & id, const QMessageManager::NotificationFilterIdSet & matchingFilterIds );
    void	messageRemoved ( const QMessageId & id, const QMessageManager::NotificationFilterIdSet & matchingFilterIds );
    void	messageUpdated ( const QMessageId & id, const QMessageManager::NotificationFilterIdSet & matchingFilterIds );

    void  getNewMessageList();

    private:
    QMessageManager *messageManager;
    int m_limit;
    QString cellphone;
    QMessageManager::NotificationFilterId filterid;

    QtMobility::QMessageIdList ids;
    bool    isInitialed;
};

#endif // QFETIONSMS_H
