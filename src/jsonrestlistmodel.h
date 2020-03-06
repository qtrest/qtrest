#ifndef QTREST_JSONRESTLISTMODEL_H
#define QTREST_JSONRESTLISTMODEL_H

#include "abstractjsonrestlistmodel.h"
#include <QtQml>
#include "base/qtrest_global.h"
#include "base/requests.h"

namespace Rest {

class QTREST_EXPORT JsonRestListModel : public AbstractJsonRestListModel
{
    Q_OBJECT

public:
    explicit JsonRestListModel(QObject *parent = nullptr);

    Q_PROPERTY(Requests *requests READ requests)

    static void declareQML()
    {
        AbstractJsonRestListModel::declareQML();
        qmlRegisterType<Requests>("com.github.qtrest.requests", 1, 0, "Requests");
        qmlRegisterType<JsonRestListModel>("com.github.qtrest.jsonrestlistmodel", 1, 0, "JsonRestListModel");
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

} // namespace Rest

#endif // QTREST_JSONRESTLISTMODEL_H
