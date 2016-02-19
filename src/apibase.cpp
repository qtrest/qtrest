#include "apibase.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

APIBase::APIBase(QObject *parent) : QObject(parent), m_authTokenHeader("Authorization"), m_acceptHeader("Accept")
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply *)),
            this, SLOT(replyFinished(QNetworkReply *)));
}

void APIBase::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->error() << reply->errorString();
        emit replyError(reply, reply->error(), reply->errorString());
    }
}

void APIBase::replyError(QNetworkReply::NetworkError error)
{
    qDebug() << "Error" << error;
}

void APIBase::slotSslErrors(QList<QSslError> errors)
{
    qDebug() << errors;
}

void APIBase::setRawHeaders(QNetworkRequest *request)
{
    request->setRawHeader(acceptHeader(), accept());
    request->setRawHeader(authTokenHeader(), authToken());
}

void APIBase::connectReplyToErrors(QNetworkReply *reply)
{
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(replyError(QNetworkReply::NetworkError)));

    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));
}

bool APIBase::checkReplyIsError(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->rawHeaderList();
        qDebug() << reply->bytesAvailable() << reply->errorString();
        return true;
    } else {
        return false;
    }
}

QNetworkReply *APIBase::get(QUrl url)
{
    QNetworkRequest request(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->get(request);
    connectReplyToErrors(reply);
    return reply;
}

