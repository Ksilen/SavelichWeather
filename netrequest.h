#ifndef NETREQUEST_H
#define NETREQUEST_H

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

class NetRequest : public QObject {
    Q_OBJECT
public:
    NetRequest();
    QJsonDocument jDoc;
    void answerAPI(QString ForRequest);

public slots:
    void managerFinished(QNetworkReply* reply);

signals:
    void replyDone();
    void replyNotDone();
    void apiKeyNotValid();//когда API Key не верен

private:
    QNetworkAccessManager* manage = new QNetworkAccessManager;
    QNetworkRequest request;
};

#endif // NETREQUEST_H
