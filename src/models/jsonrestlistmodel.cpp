#include "jsonrestlistmodel.h"

JsonRestListModel::JsonRestListModel(QObject *parent) : AbstractJsonRestListModel(parent)
{

}

QNetworkReply *JsonRestListModel::fetchMoreImpl(const QModelIndex &parent)
{
    Q_UNUSED(parent)
    return apiInstance()->handleRequest(requests()->get(), sort(), pagination(), filters(), fields());
}

QNetworkReply *JsonRestListModel::fetchDetailImpl(QString id)
{
    return apiInstance()->handleRequest(requests()->getDetails(), sort(), pagination(), filters(), fields(), id);
}

QVariantMap JsonRestListModel::preProcessItem(QVariantMap item)
{
    return item;
}
