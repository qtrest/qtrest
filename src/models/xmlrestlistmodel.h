#ifndef XMLRESTLISTMODEL_H
#define XMLRESTLISTMODEL_H

#include "baserestlistmodel.h"

class QNetworkReply;

class XmlRestListModel : public BaseRestListModel
{
    Q_OBJECT
public:
    explicit XmlRestListModel(QObject *parent = 0);

    Q_PROPERTY(QString rootElement READ rootElement WRITE setRootElement NOTIFY rootElementChanged)
    Q_PROPERTY(QString itemElement READ itemElement WRITE setItemElement NOTIFY itemElementChanged)

    QString rootElement() const
    {
        return m_rootElement;
    }

    QString itemElement() const
    {
        return m_itemElement;
    }

public slots:
    void setRootElement(QString rootElement)
    {
        if (m_rootElement == rootElement)
            return;

        m_rootElement = rootElement;
        emit rootElementChanged(rootElement);
    }

    void setItemElement(QString itemElement)
    {
        if (m_itemElement == itemElement)
            return;

        m_itemElement = itemElement;
        emit itemElementChanged(itemElement);
    }

signals:
    void rootElementChanged(QString rootElement);
    void itemElementChanged(QString itemElement);

private:
    //for get list
    QVariantList getVariantList(QByteArray bytes);

    //for get details for one element
    QVariantMap getVariantMap(QByteArray bytes);
    QString m_rootElement;
    QString m_itemElement;
};

#endif // XMLRESTLISTMODEL_H
