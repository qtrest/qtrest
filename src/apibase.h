#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>
#include <QNetworkReply>

class QNetworkAccessManager;
class QNetworkReply;

class APIBase : public QObject
{
    Q_OBJECT
public:
    explicit APIBase(QObject *parent = 0);
    bool checkReplyIsError(QNetworkReply *reply);

    Q_PROPERTY(QByteArray accept READ accept WRITE setAccept NOTIFY acceptChanged)
    Q_PROPERTY(QByteArray acceptHeader READ acceptHeader WRITE setAcceptHeader NOTIFY acceptHeaderChanged)
    //Specify Auth token for each request. Set this before run your requests (You may use Basic auth and Bearer token auth)
    Q_PROPERTY(QByteArray baseUrl READ baseUrl WRITE setBaseUrl NOTIFY baseUrlChanged)
    //Specify Auth token for each request. Set this before run your requests (You may use Basic auth and Bearer token auth)
    Q_PROPERTY(QByteArray authToken READ authToken WRITE setAuthToken NOTIFY authTokenChanged)
    //Specify Auth token header name. Default is 'Authorization' (You may use Basic auth and Bearer token auth)
    Q_PROPERTY(QByteArray authTokenHeader READ authTokenHeader WRITE setAuthTokenHeader NOTIFY authTokenHeaderChanged)
    //--------------------

    QByteArray accept() const
    {
        return m_accept;
    }

    QByteArray baseUrl() const
    {
        return m_baseUrl;
    }

    QByteArray acceptHeader() const
    {
        return m_acceptHeader;
    }

    QByteArray authToken() const
    {
        return m_authToken;
    }

    QByteArray authTokenHeader() const
    {
        return m_authTokenHeader;
    }

public slots:
    void setAccept(QString accept)
    {
        QByteArray newData;
        newData.append(accept);

        if (m_accept == newData)
            return;

        m_accept = newData;
        emit acceptChanged(newData);
    }

    void setBaseUrl(QByteArray baseUrl)
    {
        if (m_baseUrl == baseUrl)
            return;

        m_baseUrl = baseUrl;
        emit baseUrlChanged(baseUrl);
    }

    void setAcceptHeader(QByteArray acceptHeader)
    {
        if (m_acceptHeader == acceptHeader)
            return;

        m_acceptHeader = acceptHeader;
        emit acceptHeaderChanged(acceptHeader);
    }

    void setAuthToken(QByteArray authToken)
    {
        if (m_authToken == authToken)
            return;

        m_authToken = authToken;
        emit authTokenChanged(authToken);
    }

    void setAuthTokenHeader(QByteArray authTokenHeader)
    {
        if (m_authTokenHeader == authTokenHeader)
            return;

        m_authTokenHeader = authTokenHeader;
        emit authTokenHeaderChanged(authTokenHeader);
    }

signals:
    void replyError(QNetworkReply *reply, QNetworkReply::NetworkError error, QString errorString);
    void acceptChanged(QByteArray accept);
    void baseUrlChanged(QByteArray baseUrl);
    void acceptHeaderChanged(QByteArray acceptHeader);

    void authTokenChanged(QByteArray authToken);

    void authTokenHeaderChanged(QByteArray authTokenHeader);

protected:
    QNetworkReply *get(QUrl url);

    QNetworkAccessManager *manager;

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
