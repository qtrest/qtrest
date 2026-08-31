#ifndef DETAILSMODEL_H
#define DETAILSMODEL_H

#include <QSortFilterProxyModel>
#include "qtrest_global.h"

class QTREST_EXPORT DetailsModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    DetailsModel();

    bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const override;
    void invalidateModel();
};

#endif // DETAILSMODEL_H
