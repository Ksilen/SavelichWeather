#ifndef NETREQUEST_H
#define NETREQUEST_H

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

class NetRequest : public QObject {
    Q_OBJECT
public:
    NetRequest();
    virtual ~NetRequest();
    void answerAPI(QString ForRequest);
    QJsonDocument jDoc;
    QNetworkConfigurationManager netManage;
    QList<QNetworkConfiguration> activeConfigs;
    QNetworkAccessManager* manage = new QNetworkAccessManager;
public slots:
    void managerFinished(QNetworkReply* reply);

signals:
    void replyDone();
    void replyNotDone();
    void apiKeyNotValid(); //когда API Key не верен

private:
    QNetworkRequest request;
};

#endif // NETREQUEST_H
