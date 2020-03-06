#ifndef QTREST_ABSJSONRESTLISTMODEL_H
#define QTREST_ABSJSONRESTLISTMODEL_H

#include "base/qtrest_global.h"
#include "base/baserestlistmodel.h"

namespace Rest {

class QTREST_EXPORT AbstractJsonRestListModel : public BaseRestListModel
{
    Q_OBJECT
public:
    explicit AbstractJsonRestListModel(QObject *parent = nullptr);
    virtual ~AbstractJsonRestListModel() = default;

protected:
    //for get list
    virtual QVariantList getVariantList(QByteArray bytes);
    //for get details for one element
    virtual QVariantMap getVariantMap(QByteArray bytes);
};

} // namespace Rest

#endif // QTREST_ABSJSONRESTLISTMODEL_H
