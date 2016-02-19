#ifndef BASERESTLISTMODEL_H
#define BASERESTLISTMODEL_H

#include "api/api.h"
#include <QAbstractListModel>
//#include <QtQml>
#include "restitem.h"
#include "pagination.h"
#include "detailsmodel.h"

class QNetworkReply;
class DetailsModel;

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

    //Specify pagination
    Q_PROPERTY(Pagination *pagination READ pagination)

    //Specify filters parametres
    Q_PROPERTY(QVariantMap filters READ filters WRITE setFilters NOTIFY filtersChanged)
    //Specify fields parameter
    Q_PROPERTY(QStringList fields READ fields WRITE setFields NOTIFY fieldsChanged)
    //Specify Accept header for application/json or application/xml
    Q_PROPERTY(QByteArray accept READ accept WRITE setAccept NOTIFY acceptChanged)

    //identify column name, role, last fetched detail and detailModel
    Q_PROPERTY(QString idField READ idField WRITE setIdField NOTIFY idFieldChanged)
    Q_PROPERTY(int idFieldRole READ idFieldRole)
    Q_PROPERTY(QString fetchDetailLastId READ fetchDetailLastId)
    Q_PROPERTY(DetailsModel *detailsModel READ detailsModel)

    //load status and result code
    Q_PROPERTY(LoadingStatus loadingStatus READ loadingStatus WRITE setLoadingStatus NOTIFY loadingStatusChanged)
    Q_PROPERTY(QString loadingErrorString READ loadingErrorString WRITE setLoadingErrorString NOTIFY loadingErrorStringChanged)
    Q_PROPERTY(QNetworkReply::NetworkError loadingErrorCode READ loadingErrorCode WRITE setLoadingErrorCode NOTIFY loadingErrorCodeChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(CanFetchMorePolicy canFetchMorePolicy READ canFetchMorePolicy WRITE setCanFetchMorePolicy NOTIFY canFetchMorePolicyChanged)

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

    //policy for manage max elements for ListView. Default is 'ByPageCountHeader'
    enum CanFetchMorePolicy {
        ByPageCountHeader,
        ByTotalCountHeader,
        Infinity,
        Manual
    };

    Q_ENUMS(LoadingStatus)
    Q_ENUMS(CanFetchMorePolicy)

    static void declareQML();

    QVariant data(const QModelIndex &index, int role) const;

    QStringList sort() const
    {
        return m_sort;
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

    QString fetchDetailLastId() const
    {
        return m_fetchDetailLastId;
    }

    DetailsModel *detailsModel()
    {
        return &m_detailsModel;
    }

    Pagination *pagination()
    {
        return &m_pagination;
    }

    QByteArray accept() const
    {
        return restapi.accept();
    }

    int count() const
    {
        return m_items.count();
    }

    CanFetchMorePolicy canFetchMorePolicy() const
    {
        return m_canFetchMorePolicy;
    }

signals:
    void countChanged();
    void sortChanged(QStringList sort);
    void loadingStatusChanged(LoadingStatus loadingStatus);
    void filtersChanged(QVariantMap filters);
    void loadingErrorStringChanged(QString loadingErrorString);
    void loadingErrorCodeChanged(QNetworkReply::NetworkError loadingErrorCode);
    void fieldsChanged(QStringList fields);
    void idFieldChanged(QString idField);
    void acceptChanged(QByteArray accept);
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

    void setCanFetchMorePolicy(CanFetchMorePolicy canFetchMorePolicy)
    {
        if (m_canFetchMorePolicy == canFetchMorePolicy) {
            return;
        }

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
    void append(RestItem item);
    void generateRoleNames();
    void generateDetailsRoleNames(QVariantMap item);
    RestItem findItemById(QString id);

    //TODO fabric method to each items
    RestItem createItem(QVariantMap value);
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

    QList<RestItem> m_items;
    QStringList m_fields;
    QString m_idField;

    QStringList m_sort;
    LoadingStatus m_loadingStatus;
    QVariantMap m_filters;
    QString m_loadingErrorString;
    QNetworkReply::NetworkError m_loadingErrorCode;
    QString m_fetchDetailLastId;
    CanFetchMorePolicy m_canFetchMorePolicy;

    DetailsModel m_detailsModel;
    Pagination m_pagination;
};

#endif // BASERESTLISTMODEL_H
