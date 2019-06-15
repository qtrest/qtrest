#ifndef APIBASE_H
#define APIBASE_H

#include <QtCore/QtCore>
#include <QtNetwork/QtNetwork>
#include "pagination.h"

class QNetworkAccessManager;
class QNetworkReply;
class QHttpMultiPart;

class APIBase : public QObject
{
    Q_OBJECT
public:

    enum class KnownHeaders : int
    {
        ContentType = 0,
        Accept = 1,
        Authorization = 2
    };

    explicit APIBase(QObject *parent = nullptr);
    virtual ~APIBase();

    Q_PROPERTY(QByteArray baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)

    QByteArray baseUrl() const;
    QByteArray knownHeaderValue(KnownHeaders code);
    void removeKnownHeaderValue(KnownHeaders code);
    void removeExtraHeaderValue(QByteArray header);

    virtual QNetworkReply *handleRequest(QString path, QStringList sort, Pagination *pagination,
                                         QVariantMap filters = QVariantMap(),
                                         QStringList fields = QStringList(),
                                         QStringList expand = QStringList(),
                                         QString id = 0)
    {
        Q_UNUSED(path)
        Q_UNUSED(sort)
        Q_UNUSED(pagination)
        Q_UNUSED(filters)
        Q_UNUSED(fields)
        Q_UNUSED(expand)
        Q_UNUSED(id)
        return nullptr;
    }

    bool checkReplyIsError(QNetworkReply *reply);

public slots:
    void setBaseUrl(QByteArray baseUrl);
    void setKnownHeaderName(KnownHeaders code, QByteArray name);
    void setKnownHeaderValue(KnownHeaders code, QByteArray value);
    void setExtraHeaderValue(QByteArray header, QByteArray token);

signals:
    void replyError(QNetworkReply *reply, QNetworkReply::NetworkError error, QString errorString);
    void baseUrlChanged(QByteArray baseUrl);

protected:
    QNetworkReply *get(QUrl url);
    QNetworkReply *post(QUrl url);
    QNetworkReply *post(QUrl url, QIODevice *data);
    QNetworkReply *post(QUrl url, const QByteArray &data);
    QNetworkReply *post(QUrl url, QHttpMultiPart *multiPart);
    QNetworkReply *put(QUrl url);
    QNetworkReply *put(QUrl url, QIODevice *data);
    QNetworkReply *put(QUrl url, const QByteArray &data);
    QNetworkReply *put(QUrl url, QHttpMultiPart *multiPart);
    QNetworkReply *patch(QUrl url);
    QNetworkReply *patch(QUrl url, QIODevice *data);
    QNetworkReply *patch(QUrl url, const QByteArray &data);
    QNetworkReply *patch(QUrl url, QHttpMultiPart *multiPart);
    QNetworkReply *deleteResource(QUrl url);
    QNetworkReply *head(QUrl url);
    QNetworkReply *options(QUrl url);

    virtual void setRequestHeaders(QNetworkRequest *request);
    void connectReplyToErrors(QNetworkReply *reply);

protected slots:
    void replyFinished(QNetworkReply *reply);
    void handleReplyError(QNetworkReply::NetworkError error);
    void handleSslErrors(QList<QSslError> errors);

protected:
    QNetworkAccessManager m_nam;

private:
    QByteArray m_baseUrl;
    QMap<KnownHeaders, QByteArray> m_knownHeadersNames;
    QMap<KnownHeaders, QByteArray> m_knownHeadersValues;
    QMap<QByteArray, QByteArray> m_extraHeadersValues;
};

#endif // APIBASE_H
