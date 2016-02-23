#include "baserestlistmodel.h"

BaseRestListModel::BaseRestListModel(QObject *parent) : QAbstractListModel(parent), m_sort("-id"),
    m_roleNamesIndex(0), m_detailsRoleNamesIndex(0), m_loadingStatus(LoadingStatus::Idle)
{
    restapi.setAccept(accept());
    connect(&restapi,SIGNAL(replyError(QNetworkReply *, QNetworkReply::NetworkError, QString)), this, SLOT(replyError(QNetworkReply *, QNetworkReply::NetworkError, QString)));
}

void BaseRestListModel::declareQML()
{
    qRegisterMetaType<DetailsModel*>("DetailsModel*");
    qmlRegisterType<Pagination>("ru.forsk.pagination", 1, 0, "Pagination");
}

void BaseRestListModel::reload()
{
    setLoadingStatus(LoadingStatus::RequestToReload);
    this->fetchMore(QModelIndex());
}

bool BaseRestListModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    switch(pagination()->policy()) {
    case Pagination::PageNumber:
        if (pagination()->currentPage() < pagination()->pageCount()) {
            return true;
        } else {
            return false;
        }
        break;
    case Pagination::LimitOffset:
    case Pagination::Cursor:
        if (rowCount() < pagination()->totalCount()) {
            return true;
        } else {
            return false;
        }
        break;
    case Pagination::None:
        return false;
        break;
    case  Pagination::Infinity:
        return true;
        break;
    default:
        return false;
    }
}

void BaseRestListModel::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent)

    switch (loadingStatus()) {
    case LoadingStatus::RequestToReload:
        pagination()->setCurrentPage(0);
        pagination()->setOffset(0);
        pagination()->setCursorValue(0);
        setLoadingStatus(LoadingStatus::FullReloadProcessing);
        break;
    case LoadingStatus::Idle:
        setLoadingStatus(LoadingStatus::LoadMoreProcessing);
        break;
    default:
        return;
        break;
    }

    qDebug() << "fetchMore";

    switch(pagination()->policy()) {
    case Pagination::PageNumber: {
        int nextPage = pagination()->currentPage()+1;
        pagination()->setCurrentPage(nextPage);
        break;
    }
    case Pagination::LimitOffset: {
        int offset = pagination()->offset()+pagination()->limit();
        pagination()->setOffset(offset);
        break;
    }
    case Pagination::Cursor: {
        QString cursor = 0;
        if (!m_items.isEmpty()) {
            cursor = m_items.last().id();
        }
        pagination()->setCursorValue(cursor);
        break;
    }
    }

    QNetworkReply *reply = fetchMoreImpl(parent);
    connect(reply, SIGNAL(finished()), this, SLOT(fetchMoreFinished()));
}

void BaseRestListModel::fetchMoreFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (restapi.checkReplyIsError(reply) || !reply->isFinished()) {
        return;
    }

    if (this->loadingStatus() == LoadingStatus::Idle) {
        return;
    }

    qDebug() << "fetchMoreFinished";

    updateHeadersData(reply);

    QVariantList values = getVariantList(reply->readAll());

    //prepare vars
    int insertFrom = rowCount();
    int insertCount = rowCount()+values.count()-1;

    //check if we need to full reload
    if (this->loadingStatus() == LoadingStatus::FullReloadProcessing) {
        reset();
        insertFrom = rowCount();
        insertCount = values.count()-1;
    }

    //check for assertion or empty data
    if (insertCount < insertFrom) { insertCount = insertFrom; }

    if (insertCount == 0) {
        setLoadingStatus(LoadingStatus::Error);
        emit countChanged();
        qDebug() << "Nothing to insert! Please check your parser!" << count() << loadingStatus();
        return;
    }

    //append rows to model
    beginInsertRows(this->index(rowCount(), 0), insertFrom, insertCount);

    QListIterator<QVariant> i(values);
    while (i.hasNext()) {
        RestItem item = createItem(i.next().toMap());
        append(item);
    }

    //get all role names
    generateRoleNames();

    endInsertRows();

    setLoadingStatus(LoadingStatus::Idle);

    emit countChanged();
}

void BaseRestListModel::fetchDetail(QString id)
{
    qDebug() << "fetchDetail";
    m_fetchDetailLastId = id;
    RestItem item = findItemById(id);
    if (item.isUpdated()) {
        return;
    }

    switch (loadingStatus()) {
    case LoadingStatus::Idle:
        setLoadingStatus(LoadingStatus::LoadDetailsProcessing);
        break;
    default:
        return;
        break;
    }

    m_detailsModel.invalidateModel();

    QNetworkReply *reply = fetchDetailImpl(id);
    connect(reply, SIGNAL(finished()), this, SLOT(fetchDetailFinished()));
}

void BaseRestListModel::fetchDetailFinished()
{
    qDebug() << "fetchDetailFinished";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (restapi.checkReplyIsError(reply) || !reply->isFinished()) {
        return;
    }

    if (this->loadingStatus() == LoadingStatus::Idle) {
        return;
    }

    QVariantMap item = preProcessItem(getVariantMap(reply->readAll()));

    updateItem(item);

    generateDetailsRoleNames(item);

    detailsModel()->setSourceModel(this);

    setLoadingStatus(LoadingStatus::IdleDetails);
}

void BaseRestListModel::setLoadingStatus(BaseRestListModel::LoadingStatus loadingStatus)
{
    if (m_loadingStatus == loadingStatus)
        return;

    m_loadingStatus = loadingStatus;
    emit loadingStatusChanged(loadingStatus);
}

void BaseRestListModel::setAccept(QString accept)
{
    restapi.setAccept(accept);
}

void BaseRestListModel::setLoadingErrorString(QString loadingErrorString)
{
    if (m_loadingErrorString == loadingErrorString)
        return;

    m_loadingErrorString = loadingErrorString;
    emit loadingErrorStringChanged(loadingErrorString);
}

void BaseRestListModel::setLoadingErrorCode(QNetworkReply::NetworkError loadingErrorCode)
{
    if (m_loadingErrorCode == loadingErrorCode)
        return;

    m_loadingErrorCode = loadingErrorCode;
    emit loadingErrorCodeChanged(loadingErrorCode);
}

void BaseRestListModel::replyError(QNetworkReply *reply, QNetworkReply::NetworkError error, QString errorString)
{
    Q_UNUSED(reply)
    setLoadingErrorCode(error);
    setLoadingErrorString(errorString);
    setLoadingStatus(LoadingStatus::Error);
}

RestItem BaseRestListModel::createItem(QVariantMap value)
{
    return RestItem(preProcessItem(value),idField());
}

RestItem BaseRestListModel::findItemById(QString id)
{
    QListIterator<RestItem> i(m_items);
    while (i.hasNext()) {
        RestItem item = i.next();
        if (item.id() == id) {
            return item;
        }
    }
}

void BaseRestListModel::updateItem(QVariantMap value)
{
    //qDebug() << value;
    RestItem item = findItemById(fetchDetailLastId());
    int row = m_items.indexOf(item);
    item.update(value);
    emit dataChanged(index(row),index(row));
    //qDebug() << 'detail updated' << row << item.id();
}

QVariant BaseRestListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_items.count()) {
        qDebug() << "Row not found" << index.row();
        return QVariant();
    }

    RestItem item = m_items.at(index.row());
    return item.value(m_roleNames[role]);
}

QStringList BaseRestListModel::sort() const
{
    return m_sort;
}

BaseRestListModel::LoadingStatus BaseRestListModel::loadingStatus() const
{
    return m_loadingStatus;
}

QVariantMap BaseRestListModel::filters() const
{
    return m_filters;
}

QString BaseRestListModel::loadingErrorString() const
{
    return m_loadingErrorString;
}

QNetworkReply::NetworkError BaseRestListModel::loadingErrorCode() const
{
    return m_loadingErrorCode;
}

QStringList BaseRestListModel::fields() const
{
    return m_fields;
}

QString BaseRestListModel::idField() const
{
    return m_idField;
}

int BaseRestListModel::idFieldRole() const
{
    QByteArray obj;
    obj.append(idField());
    return m_roleNames.key(obj);
}

QString BaseRestListModel::fetchDetailLastId() const
{
    return m_fetchDetailLastId;
}

DetailsModel *BaseRestListModel::detailsModel()
{
    return &m_detailsModel;
}

Pagination *BaseRestListModel::pagination()
{
    return &m_pagination;
}

QByteArray BaseRestListModel::accept() const
{
    return restapi.accept();
}

int BaseRestListModel::count() const
{
    return m_items.count();
}

QHash<int, QByteArray> BaseRestListModel::roleNames() const
{
    return m_roleNames;
}

QHash<int, QByteArray> BaseRestListModel::detailsRoleNames() const
{
    return m_detailsRoleNames;
}

void BaseRestListModel::updateHeadersData(QNetworkReply *reply)
{
    //update headers data
    QByteArray currentPage;
    currentPage.append(pagination()->currentPageHeader());
    QByteArray totalCount;
    totalCount.append(pagination()->totalCountHeader());
    QByteArray pageCount;
    pageCount.append(pagination()->pageCountHeader());

    pagination()->setCurrentPage(reply->rawHeader(currentPage).toInt());
    pagination()->setTotalCount(reply->rawHeader(totalCount).toInt());
    pagination()->setPageCount(reply->rawHeader(pageCount).toInt());
    reply->deleteLater();
}

void BaseRestListModel::reset()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
}

void BaseRestListModel::append(RestItem item)
{
    m_items.append(item);
}

void BaseRestListModel::generateRoleNames()
{
    if (m_roleNamesIndex > 0) {
        return;
    }

    RestItem item = m_items[0];

    QStringList keys = item.keys();

    if (rowCount() > 0) {
        foreach (QString key, keys) {
            QByteArray k;
            k.append(key);
            if (!m_roleNames.key(k)) {
                m_roleNamesIndex++;
                m_roleNames[m_roleNamesIndex] = k;
            }
        }
    }
}

void BaseRestListModel::generateDetailsRoleNames(QVariantMap item)
{
    //qDebug() << item;
    if (m_detailsRoleNamesIndex > 0) {
        return;
    }

    QStringList keys = item.keys();

    if (rowCount() > 0) {
        foreach (QString key, keys) {
            QByteArray k;
            k.append(key);
            if (!m_detailsRoleNames.key(k)) {
                m_detailsRoleNamesIndex++;
                m_detailsRoleNames[m_detailsRoleNamesIndex] = k;
                qDebug() << key;
            }
        }
    }
}

int BaseRestListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

void BaseRestListModel::requestToReload() {
    setLoadingStatus(LoadingStatus::RequestToReload);
}

void BaseRestListModel::forceIdle() {
    setLoadingStatus(LoadingStatus::Idle);
}

void BaseRestListModel::setSort(QStringList sort)
{
    if (m_sort == sort)
        return;

    m_sort = sort;
    emit sortChanged(sort);
}

void BaseRestListModel::setFilters(QVariantMap filters)
{
    if (m_filters == filters)
        return;

    m_filters = filters;
    emit filtersChanged(filters);
}

void BaseRestListModel::setFields(QStringList fields)
{
    if (m_fields == fields)
        return;

    m_fields = fields;
    emit fieldsChanged(fields);
}

void BaseRestListModel::setIdField(QString idField)
{
    if (m_idField == idField)
        return;

    m_idField = idField;
    emit idFieldChanged(idField);
}

