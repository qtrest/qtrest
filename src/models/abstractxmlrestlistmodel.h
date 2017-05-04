#ifndef ABSXMLRESTLISTMODEL_H
#define ABSXMLRESTLISTMODEL_H

#include "baserestlistmodel.h"

class QNetworkReply;

class AbstractXmlRestListModel : public BaseRestListModel
{
    Q_OBJECT
public:
    explicit AbstractXmlRestListModel(QObject *parent = 0);

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

protected:
    //for get list
    virtual QVariantList getVariantList(QByteArray bytes);
    //for get details for one element
    virtual QVariantMap getVariantMap(QByteArray bytes);

private:
    QString m_rootElement;
    QString m_itemElement;
};

#endif // ABSXMLRESTLISTMODEL_H
