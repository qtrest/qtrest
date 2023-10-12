#ifndef JSONRESTLISTMODEL_H
#define JSONRESTLISTMODEL_H

#include "abstractjsonrestlistmodel.h"
#ifdef WITH_QML_SUPPORT
#include <QtQml>
#endif
#include "requests.h"

class JsonRestListModel : public AbstractJsonRestListModel
{
    Q_OBJECT

public:
    explicit JsonRestListModel(QObject *parent = nullptr);

    Q_PROPERTY(Requests *requests READ requests)

#ifdef WITH_QML_SUPPORT
    static void declareQML()
    {
        AbstractJsonRestListModel::declareQML();
        qmlRegisterType<Requests>("com.github.qtrest.requests", 1, 0, "Requests");
        qmlRegisterType<JsonRestListModel>("com.github.qtrest.jsonrestlistmodel", 1, 0, "JsonRestListModel");
    }
#endif

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

#endif // JSONRESTLISTMODEL_H
