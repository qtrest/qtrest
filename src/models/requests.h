#ifndef REQUESTS_H
#define REQUESTS_H

#include <QObject>

class Requests : public QObject
{
    Q_OBJECT
    QString m_get;
    QString m_getDetails;

public:
    Q_INVOKABLE explicit Requests(QObject *parent = 0);

    Q_PROPERTY(QString get READ get WRITE setGet NOTIFY getChanged)
    Q_PROPERTY(QString getDetails READ getDetails WRITE setGetDetails NOTIFY getDetailsChanged)

    QString get() const
    {
        return m_get;
    }

    QString getDetails() const
    {
        return m_getDetails;
    }

signals:
    void getChanged(QString get);
    void getDetailsChanged(QString getDetails);

public slots:
    void setGet(QString get)
    {
        if (m_get == get)
            return;

        m_get = get;
        emit getChanged(get);
    }
    void setGetDetails(QString getDetails)
    {
        if (m_getDetails == getDetails)
            return;

        m_getDetails = getDetails;
        emit getDetailsChanged(getDetails);
    }
};

#endif // REQUESTS_H
