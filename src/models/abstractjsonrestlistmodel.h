#ifndef ABSJSONRESTLISTMODEL_H
#define ABSJSONRESTLISTMODEL_H

#include "baserestlistmodel.h"

class QNetworkReply;

class AbstractJsonRestListModel : public BaseRestListModel
{
    Q_OBJECT
public:
    explicit AbstractJsonRestListModel(QObject *parent = 0);

protected:
    //for get list
    virtual QVariantList getVariantList(QByteArray bytes);
    //for get details for one element
    virtual QVariantMap getVariantMap(QByteArray bytes);
};

#endif // ABSJSONRESTLISTMODEL_H
