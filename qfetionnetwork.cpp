#include "qfetionnetwork.h"

QFetionNetwork::QFetionNetwork(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    nwam = new QNetworkAccessManager(this);
    request= QNetworkRequest(QUrl("http://166.111.73.2/~cuckoo/sms/sendsms.php"));

    connect(nwam,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
}
void QFetionNetwork::sendSMS(QString sender,QString password,QString sendto,QString message)
{
    QByteArray data;
    QUrl params;

    params.addQueryItem("cellphone", sender );
    params.addQueryItem("password", password );

    params.addQueryItem("sendto",sendto);
    params.addQueryItem("message",message.toUtf8());
    data.append(params.toString());
    data.remove(0,1);

    nwam->post(request,data);

}


void	QFetionNetwork::finished ( QNetworkReply * reply ){
    if(reply->error() == QNetworkReply::NoError)
    {
          QByteArray replyinfo = reply->readAll();


        if (replyinfo == "Success")
            qDebug() <<"SendSMS Success";
        else
            qDebug() << "SendSMS Error";

    }else
        qDebug("SendSMS Error detected!\n");

}
