#include "apibase.h"

using namespace Rest;

APIBase::APIBase(QObject *parent)
    : QObject(parent)
{
    setKnownHeaderName(KnownHeaders::ContentType,   "Content-Type");
    setKnownHeaderName(KnownHeaders::Accept,        "Accept");
    setKnownHeaderName(KnownHeaders::Authorization, "Authorization");

    connect(&m_nam, &QNetworkAccessManager::finished,
            this, &APIBase::replyFinished);
}

APIBase::~APIBase()
{

}

void APIBase::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "APIBase::replyFinished" << reply->error() << reply->errorString();
        emit replyError(reply, reply->error(), reply->errorString());
    }
}

void APIBase::handleReplyError(QNetworkReply::NetworkError error)
{
    qDebug() << "APIBase::handleReplyError" << error;
}

void APIBase::handleSslErrors(QList<QSslError> errors)
{
    qDebug() << "APIBase::handleSslErrors" << errors;
}

void APIBase::setExtraHeaderValue(QByteArray header, QByteArray token)
{
    m_extraHeadersValues[header] = token;
}

void APIBase::setRequestHeaders(QNetworkRequest *request)
{
    QMapIterator<KnownHeaders, QByteArray> it(m_knownHeadersValues);
    while (it.hasNext())
    {
        it.next();
        request->setRawHeader(m_knownHeadersNames.value(it.key()), it.value());
    }

    QMapIterator<QByteArray, QByteArray> jt(m_extraHeadersValues);
    while (jt.hasNext())
    {
        jt.next();
        request->setRawHeader(jt.key(), jt.value());
    }
}

void APIBase::connectReplyToErrors(QNetworkReply *reply)
{
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this,   SLOT(handleReplyError(QNetworkReply::NetworkError)));

    connect(reply, &QNetworkReply::sslErrors,
            this, &APIBase::handleSslErrors);
}

bool APIBase::checkReplyIsError(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->rawHeaderList();
        qDebug() << reply->bytesAvailable() << reply->errorString();
        return true;
    }
    else
    {
        return false;
    }
}

QByteArray APIBase::baseUrl() const
{
    return m_baseUrl;
}

QByteArray APIBase::knownHeaderValue(APIBase::KnownHeaders code)
{
    if (!m_knownHeadersValues.contains(code))
        return QByteArray();

    return m_knownHeadersValues[code];
}

void APIBase::removeKnownHeaderValue(APIBase::KnownHeaders code)
{
    m_knownHeadersValues.remove(code);
}

void APIBase::removeExtraHeaderValue(QByteArray header)
{
    m_extraHeadersValues.remove(header);
}

void APIBase::setBaseUrl(QByteArray baseUrl)
{
    if (m_baseUrl == baseUrl)
        return;

    m_baseUrl = baseUrl;
    emit baseUrlChanged(baseUrl);
}

void APIBase::setKnownHeaderName(APIBase::KnownHeaders code, QByteArray name)
{
    m_knownHeadersNames.insert(code, name);
}

void APIBase::setKnownHeaderValue(APIBase::KnownHeaders code, QByteArray value)
{
    m_knownHeadersValues.insert(code, value);
}

QNetworkReply *APIBase::get(QUrl url)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.get(request);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::post(QUrl url)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.sendCustomRequest(request, "POST");
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::post(QUrl url, QIODevice *data)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.post(request, data);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::post(QUrl url,const QByteArray &data)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.post(request, data);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::post(QUrl url, QHttpMultiPart *multiPart)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.post(request, multiPart);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::put(QUrl url)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.sendCustomRequest(request, "PUT");
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::put(QUrl url, QIODevice *data)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.put(request, data);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::put(QUrl url, const QByteArray &data)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.put(request, data);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::put(QUrl url, QHttpMultiPart *multiPart)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.put(request, multiPart);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::patch(QUrl url)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.sendCustomRequest(request, "PATCH");
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::patch(QUrl url, QIODevice *data)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.sendCustomRequest(request, "PATCH", data);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::patch(QUrl url, const QByteArray &data)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.sendCustomRequest(request, "PATCH", data);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::patch(QUrl url, QHttpMultiPart *multiPart)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.sendCustomRequest(request, "PATCH", multiPart);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::deleteResource(QUrl url)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.deleteResource(request);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::head(QUrl url)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.head(request);
    connectReplyToErrors(reply);
    return reply;
}

QNetworkReply *APIBase::options(QUrl url)
{
    QNetworkRequest request(url);
    setRequestHeaders(&request);

    QNetworkReply *reply = m_nam.sendCustomRequest(request, "OPTIONS");
    connectReplyToErrors(reply);
    return reply;
}
