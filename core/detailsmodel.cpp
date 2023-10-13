#include "detailsmodel.h"
#include "baserestlistmodel.h"
#include <QDebug>

DetailsModel::DetailsModel()
{

}

bool DetailsModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    bool ret = false;

    BaseRestListModel *sourceModel = static_cast<BaseRestListModel *> (this->sourceModel());

    if ( sourceModel != nullptr )
    {
        QModelIndex index = sourceModel->index( source_row, 0, source_parent );
        if ( index.isValid() )
        {
            QString id = sourceModel->data(index, sourceModel->idFieldRole()).toString();
            if (id == sourceModel->fetchDetailLastId()) {
                ret = true;
            }
        }
    }

    return ret;
}

void DetailsModel::invalidateModel() { invalidateFilter(); }
