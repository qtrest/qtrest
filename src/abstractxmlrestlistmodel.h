#ifndef QTREST_ABSXMLRESTLISTMODEL_H
#define QTREST_ABSXMLRESTLISTMODEL_H

#include "base/qtrest_global.h"
#include "base/baserestlistmodel.h"

namespace Rest {

class QTREST_EXPORT AbstractXmlRestListModel : public BaseRestListModel
{
    Q_OBJECT
public:
    explicit AbstractXmlRestListModel(QObject *parent = nullptr);
    virtual ~AbstractXmlRestListModel() = default;

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

} // namespace Rest

#endif // QTREST_ABSXMLRESTLISTMODEL_H
