#include "baserestlistmodel.h"
#include <QtQml>

BaseRestListModel::BaseRestListModel(QObject *parent) : QAbstractListModel(parent), m_roleNamesIndex(0),
    m_detailRoleNamesGenerated(false), m_sort("-id"), m_loadingStatus(LoadingStatus::Idle), m_apiInstance(nullptr), m_enableDetailsCaching(true)
{

}

void BaseRestListModel::declareQML()
{
    qRegisterMetaType<DetailsModel*>("DetailsModel*");
    qmlRegisterType<Pagination>("com.github.qtrest.pagination", 1, 0, "Pagination");
}

void BaseRestListModel::reload()
{
    setLoadingStatus(LoadingStatus::RequestToReload);
    this->fetchMore(QModelIndex());
}

bool BaseRestListModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    switch(m_pagination.policy()) {
    case Pagination::PageNumber:
        if (m_pagination.currentPage() < m_pagination.pageCount()) {
            return true;
        } else {
            return false;
        }
        break;
    case Pagination::LimitOffset:
    case Pagination::Cursor:
        if (rowCount() < m_pagination.totalCount()) {
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
        m_pagination.setCurrentPage(0);
        m_pagination.setOffset(0);
        m_pagination.setCursorValue(0);
        setLoadingStatus(LoadingStatus::FullReloadProcessing);
        break;
    case LoadingStatus::Idle:
    case LoadingStatus::IdleDetails:
        setLoadingStatus(LoadingStatus::LoadMoreProcessing);
        break;
    default:
        return;
        break;
    }

    switch(m_pagination.policy()) {
    case Pagination::PageNumber: {
        int nextPage = m_pagination.currentPage()+1;
        m_pagination.setCurrentPage(nextPage);
        break;
    }
    case Pagination::LimitOffset: {
        int offset = m_pagination.offset()+m_pagination.limit();
        m_pagination.setOffset(offset);
        break;
    }
    case Pagination::Cursor: {
        QString cursor = 0;
        if (!m_items.isEmpty()) {
            cursor = m_items.last().id();
        }
        m_pagination.setCursorValue(cursor);
        break;
    }
    default:
        break;
    }

    QNetworkReply *reply = fetchMoreImpl(parent);
    connect(reply, SIGNAL(finished()), this, SLOT(fetchMoreFinished()));
}

void BaseRestListModel::fetchMoreFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (apiInstance()->checkReplyIsError(reply) || !reply->isFinished()) {
        return;
    }

    if (this->loadingStatus() == LoadingStatus::Idle) {
        return;
    }

    updateHeadersData(reply);

    QVariantList values = getVariantList(reply->readAll());

    //prepare vars
    int insertFrom = rowCount();
    int insertCount = rowCount()+values.count();

    //check if we need to full reload
    if (this->loadingStatus() == LoadingStatus::FullReloadProcessing) {
        reset();
        insertFrom = rowCount();
        insertCount = values.count();
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
    beginInsertRows(this->index(rowCount(), 0), insertFrom, insertCount-1);

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
    m_fetchDetailLastId = id;
    RestItem item = findItemById(id);

    if (!item.isValid()) {
        qWarning() << QString("No item with id %1").arg(id);
        return;
    }

    if (enableDetailsCaching() && item.isUpdated()) {
        return;
    }

    switch (loadingStatus()) {
    case LoadingStatus::Idle:
    case LoadingStatus::IdleDetails:
        setLoadingStatus(LoadingStatus::LoadDetailsProcessing);
        break;
    default:
        return;
        break;
    }

    m_detailsModel.invalidateModel();

    // clean up the details model (QQmlPropertyMap)
    for (const QString &key : m_details.keys()) {
        m_details.clear(key);
    }

    QNetworkReply *reply = fetchDetailImpl(id);
    connect(reply, SIGNAL(finished()), this, SLOT(fetchDetailFinished()));
}

void BaseRestListModel::fetchDetailFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (apiInstance()->checkReplyIsError(reply) || !reply->isFinished()) {
        return;
    }

    if (this->loadingStatus() == LoadingStatus::Idle) {
        return;
    }

    QVariantMap item = preProcessItem(getVariantMap(reply->readAll()));

    updateItem(item);

    generateDetailsRoleNames(item);

    detailsModel()->setSourceModel(this);

    // fill up the details model (QQmlPropertyMap)
    QMapIterator<QString, QVariant> i(item);
    while (i.hasNext()) {
        i.next();
        m_details.insert(i.key(), i.value());
    }

    setLoadingStatus(LoadingStatus::IdleDetails);
}

void BaseRestListModel::setLoadingStatus(BaseRestListModel::LoadingStatus loadingStatus)
{
    if (m_loadingStatus == loadingStatus) {
        return;
    }

    m_loadingStatus = loadingStatus;
    emit loadingStatusChanged(loadingStatus);
}

void BaseRestListModel::setAccept(QString accept)
{
    apiInstance()->setAccept(accept);
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

bool BaseRestListModel::enableDetailsCaching() const
{
    return m_enableDetailsCaching;
}

void BaseRestListModel::setEnableDetailsCaching(bool enableDetailsCaching)
{
    if (m_enableDetailsCaching == enableDetailsCaching) {
        return;
    }

    m_enableDetailsCaching = enableDetailsCaching;
    emit enableDetailsCachingChanged(enableDetailsCaching);
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

    return RestItem();
}

void BaseRestListModel::updateItem(QVariantMap value)
{
    QString id = fetchDetailLastId();
    RestItem item = findItemById(id);

    if (!item.isValid()) {
        qWarning() << QString("No item with id %1").arg(id);
        return;
    }

    int row = m_items.indexOf(item);
    item.update(value);
    m_items.replace(row, item);
    emit dataChanged(index(row),index(row));
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

QStringList BaseRestListModel::expand() const
{
    return m_expand;
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

QQmlPropertyMap *BaseRestListModel::details()
{
    return &m_details;
}

Pagination *BaseRestListModel::pagination()
{
    return &m_pagination;
}

QByteArray BaseRestListModel::accept()
{
    return apiInstance()->accept();
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
    return m_roleNames;
}

void BaseRestListModel::updateHeadersData(QNetworkReply *reply)
{
    //update headers data
    QByteArray currentPage;
    currentPage.append(m_pagination.currentPageHeader());
    QByteArray totalCount;
    totalCount.append(m_pagination.totalCountHeader());
    QByteArray pageCount;
    pageCount.append(m_pagination.pageCountHeader());

    m_pagination.setCurrentPage(reply->rawHeader(currentPage).toInt());
    m_pagination.setTotalCount(reply->rawHeader(totalCount).toInt());
    m_pagination.setPageCount(reply->rawHeader(pageCount).toInt());
    reply->deleteLater();

    //todo other headers (limit offset and cursor)
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
    if (m_detailRoleNamesGenerated) { return; }

    QStringList keys = item.keys();

    if (rowCount() > 0) {
        foreach (QString key, keys) {
            QByteArray k = key.toUtf8();
            if (!m_roleNames.key(k)) {
                m_roleNamesIndex++;
                m_roleNames[m_roleNamesIndex] = k;
            }
        }
    }

    m_detailRoleNamesGenerated = true;
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

void BaseRestListModel::setExpand(QStringList expand)
{
    if (m_expand == expand)
        return;

    m_expand = expand;
    emit expandChanged(expand);
}

void BaseRestListModel::setIdField(QString idField)
{
    if (m_idField == idField)
        return;

    m_idField = idField;
    emit idFieldChanged(idField);
}

void BaseRestListModel::setApiInstance(APIBase *apiInstance)
{
    if (m_apiInstance == apiInstance)
        return;

    m_apiInstance = apiInstance;

    m_apiInstance->setAccept(accept());
    connect(m_apiInstance,SIGNAL(replyError(QNetworkReply *, QNetworkReply::NetworkError, QString)),
            this, SLOT(replyError(QNetworkReply *, QNetworkReply::NetworkError, QString)));

    emit apiInstanceChanged(apiInstance);
}

APIBase *BaseRestListModel::apiInstance()
{
    if (m_apiInstance == nullptr) {
        return new APIBase();
    }
    return m_apiInstance;
}
