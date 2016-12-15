#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "pagination.h"

class QNetworkAccessManager;
class QNetworkReply;
class QHttpMultiPart;

class APIBase : public QObject
{
    Q_OBJECT
public:
    explicit APIBase(QObject *parent = 0);
    bool checkReplyIsError(QNetworkReply *reply);

    //Accept header for JSON/XML data
    Q_PROPERTY(QByteArray accept READ accept WRITE setAccept NOTIFY acceptChanged)
    Q_PROPERTY(QByteArray acceptHeader READ acceptHeader WRITE setAcceptHeader NOTIFY acceptHeaderChanged)
    //Specify Auth token for each request. Set this before run your requests (You may use Basic auth and Bearer token auth)
    Q_PROPERTY(QByteArray baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    //Specify Auth token for each request. Set this before run your requests (You may use Basic auth and Bearer token auth)
    Q_PROPERTY(QByteArray authToken READ authToken WRITE setAuthToken NOTIFY authTokenChanged)
    //Specify Auth token header name. Default is 'Authorization' (You may use Basic auth and Bearer token auth)
    Q_PROPERTY(QByteArray authTokenHeader READ authTokenHeader WRITE setAuthTokenHeader NOTIFY authTokenHeaderChanged)
    //--------------------

    QByteArray baseUrl() const;
    QByteArray accept() const;
    QByteArray acceptHeader() const;
    QByteArray authToken() const;
    QByteArray authTokenHeader() const;

    virtual QNetworkReply *handleRequest(QString path, QStringList sort, Pagination *pagination,
                                         QVariantMap filters = QVariantMap(),
                                         QStringList fields = QStringList(), QString id = 0) {
        Q_UNUSED(path)
        Q_UNUSED(sort)
        Q_UNUSED(pagination)
        Q_UNUSED(filters)
        Q_UNUSED(fields)
        Q_UNUSED(id)
        return 0;
    };

public slots:
    void setBaseUrl(QByteArray baseUrl);
    void setAccept(QString accept);
    void setAcceptHeader(QByteArray acceptHeader);
    void setAuthToken(QByteArray authToken);
    void setAuthTokenHeader(QByteArray authTokenHeader);

signals:
    void replyError(QNetworkReply *reply, QNetworkReply::NetworkError error, QString errorString);
    void acceptChanged(QByteArray accept);
    void baseUrlChanged(QByteArray baseUrl);
    void acceptHeaderChanged(QByteArray acceptHeader);
    void authTokenChanged(QByteArray authToken);
    void authTokenHeaderChanged(QByteArray authTokenHeader);

protected:
    QNetworkReply *get(QUrl url);
    QNetworkReply *post(QUrl url, QIODevice *data);
    QNetworkReply *post(QUrl url, const QByteArray &data);
    QNetworkReply *post(QUrl url, QHttpMultiPart *multiPart);
    QNetworkReply *put(QUrl url, QIODevice *data);
    QNetworkReply *put(QUrl url, const QByteArray &data);
    QNetworkReply *put(QUrl url, QHttpMultiPart *multiPart);
    QNetworkReply *deleteResource(QUrl url);
    QNetworkReply *head(QUrl url);
    QNetworkReply *options(QUrl url);
    QNetworkReply *patch(QUrl url);

    QNetworkAccessManager *manager;

    virtual QNetworkRequest createRequest(const QUrl &url) const;

    void setRawHeaders(QNetworkRequest *request);
    void connectReplyToErrors(QNetworkReply *reply);

protected slots:
    void replyFinished(QNetworkReply *reply);
    void replyError(QNetworkReply::NetworkError error);
    void slotSslErrors(QList<QSslError> errors);

private:
    QByteArray m_accept;
    QByteArray m_baseUrl;
    QByteArray m_acceptHeader;
    QByteArray m_authToken;
    QByteArray m_authTokenHeader;
};

#endif // APIMANAGER_H
