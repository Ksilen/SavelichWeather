#include "netrequest.h"

NetRequest::NetRequest()
{
    connect(manage, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(managerFinished(QNetworkReply*)));
}
void NetRequest::managerFinished(QNetworkReply* reply) //вывод запросов к API
{
    if (reply->error()) {
        if (reply->errorString() == QString("Host requires authentication")) {
            emit apiKeyNotValid(); //когда API Key не верен
            return;
        }
        emit replyNotDone();
        return;
    }
    QString answer = reply->readAll();
    jDoc = QJsonDocument::fromJson(answer.toUtf8());
    reply->deleteLater();
    emit replyDone();
}
void NetRequest::answerAPI(QString ForRequest)
{
    request.setUrl(QUrl::fromUserInput(ForRequest));
    manage->get(request);
}
NetRequest::~NetRequest()
{
}
