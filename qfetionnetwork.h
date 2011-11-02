#ifndef QFETIONNETWORK_H
#define QFETIONNETWORK_H

#include <QDeclarativeItem>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class QFetionNetwork : public QDeclarativeItem
{
    Q_OBJECT
public:
    explicit QFetionNetwork(QDeclarativeItem *parent = 0);

    Q_INVOKABLE void sendSMS(QString sender,QString password,QString sendto,QString message);
signals:

public slots:
    void	finished ( QNetworkReply * reply );

private:
    QNetworkAccessManager *nwam;

    QNetworkRequest request;


};

#endif // QFETIONNETWORK_H
