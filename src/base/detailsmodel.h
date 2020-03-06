#ifndef QTREST_DETAILSMODEL_H
#define QTREST_DETAILSMODEL_H

#include <QSortFilterProxyModel>
#include "base/qtrest_global.h"

namespace Rest {

class QTREST_EXPORT DetailsModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    DetailsModel();

    bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const override;
    void invalidateModel();
};

} // namespace Rest

#endif // QTREST_DETAILSMODEL_H
