#ifndef XMLRESTLISTMODEL_H
#define XMLRESTLISTMODEL_H

#include "abstractxmlrestlistmodel.h"
#include <QtQml>
#include "requests.h"

class XmlRestListModel : public AbstractXmlRestListModel
{
public:
    XmlRestListModel();

    Q_PROPERTY(Requests *requests READ requests)

    static void declareQML() {
        AbstractXmlRestListModel::declareQML();
        qmlRegisterType<Requests>("com.github.qtrest.requests", 1, 0, "Requests");
        qmlRegisterType<XmlRestListModel>("com.github.qtrest.xmlrestlistmodel", 1, 0, "XmlRestListModel");
    }

    Requests *requests()
    {
        return &m_requests;
    }

protected:
    QNetworkReply *fetchMoreImpl(const QModelIndex &parent);
    QNetworkReply *fetchDetailImpl(QString id);
    QVariantMap preProcessItem(QVariantMap item);

    Requests m_requests;
};

#endif // XMLRESTLISTMODEL_H
