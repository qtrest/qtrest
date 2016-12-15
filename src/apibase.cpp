#include "apibase.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

APIBase::APIBase(QObject *parent) : QObject(parent), m_acceptHeader("Accept"), m_authTokenHeader("Authorization")
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

QByteArray APIBase::accept() const
{
    return m_accept;
}

QByteArray APIBase::baseUrl() const
{
    return m_baseUrl;
}

QByteArray APIBase::acceptHeader() const
{
    return m_acceptHeader;
}

QByteArray APIBase::authToken() const
{
    return m_authToken;
}

QByteArray APIBase::authTokenHeader() const
{
    return m_authTokenHeader;
}

void APIBase::setAccept(QString accept)
{
    QByteArray newData;
    newData.append(accept);

    if (m_accept == newData)
        return;

    m_accept = newData;
    emit acceptChanged(newData);
}

void APIBase::setBaseUrl(QByteArray baseUrl)
{
    if (m_baseUrl == baseUrl)
        return;

    m_baseUrl = baseUrl;
    emit baseUrlChanged(baseUrl);
}

void APIBase::setAcceptHeader(QByteArray acceptHeader)
{
    if (m_acceptHeader == acceptHeader)
        return;

    m_acceptHeader = acceptHeader;
    emit acceptHeaderChanged(acceptHeader);
}

void APIBase::setAuthToken(QByteArray authToken)
{
    if (m_authToken == authToken)
        return;

    m_authToken = authToken;
    emit authTokenChanged(authToken);
}

void APIBase::setAuthTokenHeader(QByteArray authTokenHeader)
{
    if (m_authTokenHeader == authTokenHeader)
        return;

    m_authTokenHeader = authTokenHeader;
    emit authTokenHeaderChanged(authTokenHeader);
}

QNetworkReply *APIBase::get(QUrl url)
{
    QNetworkRequest request = createRequest(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->get(request);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::post(QUrl url, QIODevice *data)
{
    QNetworkRequest request = createRequest(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->post(request, data);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::post(QUrl url,const QByteArray &data)
{
    QNetworkRequest request = createRequest(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->post(request, data);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::post(QUrl url, QHttpMultiPart *multiPart)
{
    QNetworkRequest request = createRequest(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->post(request, multiPart);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::put(QUrl url, QIODevice *data)
{
    QNetworkRequest request = createRequest(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->put(request, data);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::put(QUrl url, const QByteArray &data)
{
    QNetworkRequest request = createRequest(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->put(request, data);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::put(QUrl url, QHttpMultiPart *multiPart)
{
    QNetworkRequest request = createRequest(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->put(request, multiPart);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::deleteResource(QUrl url)
{
    QNetworkRequest request = createRequest(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->deleteResource(request);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::head(QUrl url)
{
    QNetworkRequest request = createRequest(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->head(request);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::options(QUrl url)
{
    QNetworkRequest request = createRequest(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->sendCustomRequest(request,"OPTIONS");
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::patch(QUrl url)
{
    QNetworkRequest request = createRequest(url);
    setRawHeaders(&request);

    QNetworkReply *reply = manager->sendCustomRequest(request,"PATCH");
    connectReplyToErrors(reply);
    return reply;
}

QNetworkRequest APIBase::createRequest(const QUrl &url) const
{
    return QNetworkRequest(url);
}
