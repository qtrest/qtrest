#ifndef BASERESTLISTMODEL_H
#define BASERESTLISTMODEL_H

#include "api/api.h"
#include <QAbstractListModel>
#include <QtQml>

class QNetworkReply;
class DetailsModel;

class Item {
public:
    explicit Item(QVariantMap object, QString idField) {
        m_object = object;
        m_idField = idField;
        m_isUpdated = false;
    }
    QVariant value(QString key) {
        return m_object.value(key);
    }
    QStringList keys() {
        return m_object.keys();
    }
    QString id() const {
        return m_object.value(m_idField).toString();
    }
    bool isUpdated() {
        return m_isUpdated;
    }

    void update (QVariantMap value) {
        QMapIterator<QString, QVariant> i(value);
        while (i.hasNext()) {
            i.next();
            m_object.insert(i.key(), i.value());
        }
        m_isUpdated = true;
    }

    bool operator==(const Item &other) {
        return id() == other.id();
    }
private:
    QVariantMap m_object;
    QString m_idField;
    bool m_isUpdated;
};

class BaseRestListModel : public QAbstractListModel
{
    Q_OBJECT
    friend DetailsModel;
public:
    BaseRestListModel(QObject *parent = 0);

    //--------------------
    //Standard HATEOAS REST API params (https://en.wikipedia.org/wiki/HATEOAS, for example: https://github.com/yiisoft/yii2/blob/master/docs/guide-ru/rest-quick-start.md)
    //Specify sorting fields
    Q_PROPERTY(QStringList sort READ sort WRITE setSort NOTIFY sortChanged)
    //Specify perPage count
    Q_PROPERTY(int perPage READ perPage WRITE setPerPage NOTIFY perPageChanged)
    //Read only incremental currentPage
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    //Specify current page header. Default is incremental X-Pagination-Current-Page, increment by call fetchMore (X-Pagination-Page-Count)
    Q_PROPERTY(QString currentPageHeader READ currentPageHeader WRITE setCurrentPageHeader NOTIFY currentPageHeaderChanged)
    //Read only max total records count from X-Pagination-Total-Count
    Q_PROPERTY(int totalCount READ totalCount WRITE setTotalCount NOTIFY totalCountChanged)
    //Max total records count. Default is X-Pagination-Total-Count
    Q_PROPERTY(QString totalCountHeader READ totalCountHeader WRITE setTotalCountHeader NOTIFY totalCountHeaderChanged)
    //Read only max page count from X-Pagination-Page-Count
    Q_PROPERTY(int pageCount READ pageCount WRITE setPageCount NOTIFY pageCountChanged)
    //Max page count. Default X-Pagination-Page-Count
    Q_PROPERTY(QString pageCountHeader READ pageCountHeader WRITE setPageCountHeader NOTIFY pageCountHeaderChanged)
    //Specify filters parametres
    Q_PROPERTY(QVariantMap filters READ filters WRITE setFilters NOTIFY filtersChanged)
    //Specify fields parameter
    Q_PROPERTY(QStringList fields READ fields WRITE setFields NOTIFY fieldsChanged)
    //Specify Accept header for application/json or application/xml
    Q_PROPERTY(QByteArray accept READ accept WRITE setAccept NOTIFY acceptChanged)

    //identify column name, role, last fetched detail and detailModel
    Q_PROPERTY(QString idField READ idField WRITE setIdField NOTIFY idFieldChanged)
    Q_PROPERTY(int idFieldRole READ idFieldRole)
    Q_PROPERTY(QString fetchDetailId READ fetchDetailId)
    Q_PROPERTY(DetailsModel *detailsModel READ detailsModel)

    //load status and result code
    Q_PROPERTY(LoadingStatus loadingStatus READ loadingStatus WRITE setLoadingStatus NOTIFY loadingStatusChanged)
    Q_PROPERTY(QString loadingErrorString READ loadingErrorString WRITE setLoadingErrorString NOTIFY loadingErrorStringChanged)
    Q_PROPERTY(QNetworkReply::NetworkError loadingErrorCode READ loadingErrorCode WRITE setLoadingErrorCode NOTIFY loadingErrorCodeChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(CanFetchMorePolicy canFetchMorePolicy READ canFetchMorePolicy WRITE setCanFetchMorePolicy NOTIFY canFetchMorePolicyChanged)

    Q_ENUMS(LoadingStatus)

    //current status of model
    enum LoadingStatus {
        Idle,
        IdleDetails,
        RequestToReload,
        FullReloadProcessing,
        LoadMoreProcessing,
        LoadDetailsProcessing,
        Error
    };

    Q_ENUMS(CanFetchMorePolicy)

    //policy for manage max elements for ListView. Default is 'ByPageCountHeader'
    enum CanFetchMorePolicy {
        ByPageCountHeader,
        ByTotalCountHeader,
        Infinity,
        Manual
    };

    static void declareQML();

    QVariant data(const QModelIndex &index, int role) const;

    QStringList sort() const
    {
        return m_sort;
    }

    int perPage() const
    {
        return m_perPage;
    }

    int currentPage() const
    {
        return m_currentPage;
    }

    int totalCount() const
    {
        return m_totalCount;
    }

    int pageCount() const
    {
        return m_pageCount;
    }

    LoadingStatus loadingStatus() const
    {
        return m_loadingStatus;
    }

    QVariantMap filters() const
    {
        return m_filters;
    }

    QString loadingErrorString() const
    {
        return m_loadingErrorString;
    }

    QNetworkReply::NetworkError loadingErrorCode() const
    {
        return m_loadingErrorCode;
    }

    QStringList fields() const
    {
        return m_fields;
    }

    QString idField() const
    {
        return m_idField;
    }

    int idFieldRole() const
    {
        QByteArray obj;
        obj.append(idField());
        return m_roleNames.key(obj);
    }

    QString fetchDetailId() const
    {
        return m_fetchDetailId;
    }

    DetailsModel *detailsModel() const
    {
        return m_detailsModel;
    }

    QByteArray accept() const
    {
        return restapi.accept();
    }

    int count() const
    {
        return m_items.count();
    }
    QString pageCountHeader() const
    {
        return m_pageCountHeader;
    }

    QString totalCountHeader() const
    {
        return m_totalCountHeader;
    }

    QString currentPageHeader() const
    {
        return m_currentPageHeader;
    }

    CanFetchMorePolicy canFetchMorePolicy() const
    {
        return m_canFetchMorePolicy;
    }

signals:
    void countChanged();
    void sortChanged(QStringList sort);
    void perPageChanged(int perPage);
    void currentPageChanged(int currentPage);
    void totalCountChanged(int totalCount);
    void pageCountChanged(int pageCount);
    void loadingStatusChanged(LoadingStatus loadingStatus);
    void filtersChanged(QVariantMap filters);
    void loadingErrorStringChanged(QString loadingErrorString);
    void loadingErrorCodeChanged(QNetworkReply::NetworkError loadingErrorCode);
    void fieldsChanged(QStringList fields);
    void idFieldChanged(QString idField);
    void acceptChanged(QByteArray accept);
    void pageCountHeaderChanged(QString pageCountHeader);
    void totalCountHeaderChanged(QString totalCountHeader);
    void currentPageHeaderChanged(QString currentPageHeader);
    void canFetchMorePolicyChanged(CanFetchMorePolicy canFetchMorePolicy);

public slots:
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);
    void reload();
    void fetchDetail(QString id);
    void replyError(QNetworkReply *reply, QNetworkReply::NetworkError error, QString errorString);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    void requestToReload() {
        setLoadingStatus(LoadingStatus::RequestToReload);
    }

    void forceIdle() {
        setLoadingStatus(LoadingStatus::Idle);
    }

    void setSort(QStringList sort)
    {
        if (m_sort == sort)
            return;

        m_sort = sort;
        emit sortChanged(sort);
    }

    void setPerPage(int perPage)
    {
        if (m_perPage == perPage)
            return;

        m_perPage = perPage;
        emit perPageChanged(perPage);
    }

    void setFilters(QVariantMap filters)
    {
        if (m_filters == filters)
            return;

        m_filters = filters;
        emit filtersChanged(filters);
    }

    void setFields(QStringList fields)
    {
        if (m_fields == fields)
            return;

        m_fields = fields;
        emit fieldsChanged(fields);
    }

    void setIdField(QString idField)
    {
        if (m_idField == idField)
            return;

        m_idField = idField;
        emit idFieldChanged(idField);
    }

    void setPageCountHeader(QString pageCountHeader)
    {
        if (m_pageCountHeader == pageCountHeader)
            return;

        m_pageCountHeader = pageCountHeader;
        emit pageCountHeaderChanged(pageCountHeader);
    }

    void setTotalCountHeader(QString totalCountHeader)
    {
        if (m_totalCountHeader == totalCountHeader)
            return;

        m_totalCountHeader = totalCountHeader;
        emit totalCountHeaderChanged(totalCountHeader);
    }

    void setCurrentPageHeader(QString currentPageHeader)
    {
        if (m_currentPageHeader == currentPageHeader)
            return;

        m_currentPageHeader = currentPageHeader;
        emit currentPageHeaderChanged(currentPageHeader);
    }

    void setCanFetchMorePolicy(CanFetchMorePolicy canFetchMorePolicy)
    {
        if (m_canFetchMorePolicy == canFetchMorePolicy)
            return;

        m_canFetchMorePolicy = canFetchMorePolicy;
        emit canFetchMorePolicyChanged(canFetchMorePolicy);
    }

protected:
    virtual QNetworkReply *fetchMoreImpl(const QModelIndex &parent) = 0;
    virtual QVariantMap preProcessItem(QVariantMap item) = 0;
    virtual QNetworkReply *fetchDetailImpl(QString id) = 0;

    //for get list
    virtual QVariantList getVariantList(QByteArray bytes) = 0;

    //for get details for one element
    virtual QVariantMap getVariantMap(QByteArray bytes) = 0;

    void updateHeadersData(QNetworkReply *reply);
    void clearForReload();
    void append(Item item);
    void generateRoleNames();
    void generateDetailsRoleNames(QVariantMap item);
    Item findItemById(QString id);

    //TODO fabric method to each items
    Item createItem(QVariantMap value);
    void updateItem(QVariantMap value);

    QHash<int, QByteArray> roleNames() const;
    QHash<int, QByteArray> detailsRoleNames() const;

protected slots:
    void fetchMoreFinished();
    void fetchDetailFinished();

    void setLoadingStatus(LoadingStatus loadingStatus)
    {
        if (m_loadingStatus == loadingStatus)
            return;

        m_loadingStatus = loadingStatus;
        emit loadingStatusChanged(loadingStatus);
    }

    void setCurrentPage(int currentPage)
    {
        if (m_currentPage == currentPage)
            return;

        m_currentPage = currentPage;
        emit currentPageChanged(currentPage);
    }

    void setTotalCount(int totalCount)
    {
        if (m_totalCount == totalCount)
            return;

        m_totalCount = totalCount;
        emit totalCountChanged(totalCount);
    }

    void setPageCount(int pageCount)
    {
        if (m_pageCount == pageCount)
            return;

        m_pageCount = pageCount;
        emit pageCountChanged(pageCount);
    }

    void setAccept(QString accept)
    {
        restapi.setAccept(accept);
    }

    void setLoadingErrorString(QString loadingErrorString)
    {
        if (m_loadingErrorString == loadingErrorString)
            return;

        m_loadingErrorString = loadingErrorString;
        emit loadingErrorStringChanged(loadingErrorString);
    }

    void setLoadingErrorCode(QNetworkReply::NetworkError loadingErrorCode)
    {
        if (m_loadingErrorCode == loadingErrorCode)
            return;

        m_loadingErrorCode = loadingErrorCode;
        emit loadingErrorCodeChanged(loadingErrorCode);
    }

private:

    QHash<int, QByteArray> m_roleNames;
    int m_roleNamesIndex;

    QHash<int, QByteArray> m_detailsRoleNames;
    int m_detailsRoleNamesIndex;

    QList<Item> m_items;
    QStringList m_fields;
    QString m_idField;

    QStringList m_sort;
    int m_perPage;
    int m_currentPage;
    int m_totalCount;
    int m_pageCount;
    LoadingStatus m_loadingStatus;
    QVariantMap m_filters;
    QString m_loadingErrorString;
    QNetworkReply::NetworkError m_loadingErrorCode;
    QString m_fetchDetailId;
    DetailsModel *m_detailsModel;
    QString m_pageCountHeader;
    QString m_totalCountHeader;
    QString m_currentPageHeader;
    CanFetchMorePolicy m_canFetchMorePolicy;
};

#endif // BASERESTLISTMODEL_H
