#include "xmlrestlistmodel.h"

XmlRestListModel::XmlRestListModel()
{

}

QNetworkReply *XmlRestListModel::fetchMoreImpl(const QModelIndex &parent)
{
    Q_UNUSED(parent)
    return apiInstance()->handleRequest(requests()->get(), sort(), pagination(), filters(), fields());
}

QNetworkReply *XmlRestListModel::fetchDetailImpl(QString id)
{
    return apiInstance()->handleRequest(requests()->getDetails(), sort(), pagination(), filters(), fields(), id);
}

QVariantMap XmlRestListModel::preProcessItem(QVariantMap item)
{
    return item;
}
