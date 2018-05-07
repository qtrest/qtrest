#ifndef BASERESTLISTMODEL_H
#define BASERESTLISTMODEL_H

#include <QAbstractListModel>
#include <QQmlPropertyMap>
#include "restitem.h"
#include "pagination.h"
#include "detailsmodel.h"
#include "apibase.h"

class QNetworkReply;
class DetailsModel;

class BaseRestListModel : public QAbstractListModel
{
    Q_OBJECT
    friend DetailsModel;
public:
    BaseRestListModel(QObject *parent = 0);

    Q_PROPERTY(APIBase *api READ apiInstance WRITE setApiInstance NOTIFY apiInstanceChanged)

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
    //Specify expand parameter
    Q_PROPERTY(QStringList expand READ expand WRITE setExpand NOTIFY expandChanged)
    //Specify Accept header for application/json or application/xml
    Q_PROPERTY(QByteArray accept READ accept WRITE setAccept NOTIFY acceptChanged)

    //identify column name, role, last fetched detail and detailModel
    Q_PROPERTY(QString idField READ idField WRITE setIdField NOTIFY idFieldChanged)
    Q_PROPERTY(bool enableDetailsCaching READ enableDetailsCaching WRITE setEnableDetailsCaching NOTIFY enableDetailsCachingChanged)
    Q_PROPERTY(int idFieldRole READ idFieldRole)
    Q_PROPERTY(QString fetchDetailLastId READ fetchDetailLastId)
    Q_PROPERTY(DetailsModel *detailsModel READ detailsModel)
    Q_PROPERTY(QQmlPropertyMap *details READ details CONSTANT)

    //load status and result code
    Q_PROPERTY(LoadingStatus loadingStatus READ loadingStatus WRITE setLoadingStatus NOTIFY loadingStatusChanged)
    Q_PROPERTY(QString loadingErrorString READ loadingErrorString WRITE setLoadingErrorString NOTIFY loadingErrorStringChanged)
    Q_PROPERTY(QNetworkReply::NetworkError loadingErrorCode READ loadingErrorCode WRITE setLoadingErrorCode NOTIFY loadingErrorCodeChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

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

    Q_ENUMS(LoadingStatus)

    static void declareQML();

    //Properties GET methods
    QStringList sort() const;
    LoadingStatus loadingStatus() const;
    QVariantMap filters() const;
    QString loadingErrorString() const;
    QNetworkReply::NetworkError loadingErrorCode() const;
    QStringList fields() const;
    QStringList expand() const;
    QString idField() const;
    int idFieldRole() const;
    QString fetchDetailLastId() const;
    DetailsModel *detailsModel();
    QQmlPropertyMap *details();
    Pagination *pagination();
    QByteArray accept();
    int count() const;

    //Overloaded system methdos
    QVariant data(const QModelIndex &index, int role) const;

    bool enableDetailsCaching() const;
    void setEnableDetailsCaching(bool enableDetailsCaching);

signals:
    //Properties signals
    void countChanged();
    void sortChanged(QStringList sort);
    void loadingStatusChanged(LoadingStatus loadingStatus);
    void filtersChanged(QVariantMap filters);
    void loadingErrorStringChanged(QString loadingErrorString);
    void loadingErrorCodeChanged(QNetworkReply::NetworkError loadingErrorCode);
    void fieldsChanged(QStringList fields);
    void expandChanged(QStringList expand);
    void idFieldChanged(QString idField);
    void acceptChanged(QByteArray accept);
    void apiInstanceChanged(APIBase *apiInstance);
    void enableDetailsCachingChanged(bool enableDetailsCaching);

public slots:
    void reload();
    void fetchDetail(QString id);
    void replyError(QNetworkReply *reply, QNetworkReply::NetworkError error, QString errorString);

    void requestToReload();
    void forceIdle();

    //Overloaded system methdos
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    //Properties public SET methods
    void setSort(QStringList sort);
    void setFilters(QVariantMap filters);
    void setFields(QStringList fields);
    void setExpand(QStringList expand);
    void setIdField(QString idField);

    void setApiInstance(APIBase *apiInstance);

protected:
    //reimplement this for call specific API method GET list
    virtual QNetworkReply *fetchMoreImpl(const QModelIndex &parent) = 0;
    //reimplement this for call specific API method GET details of record by ID
    virtual QNetworkReply *fetchDetailImpl(QString id) = 0;
    //reimplenet this for prepropcessing each item before add it to model
    virtual QVariantMap preProcessItem(QVariantMap item) = 0;
    //for parse list, reimplemented in JSON and XML models
    virtual QVariantList getVariantList(QByteArray bytes) = 0;
    //for parse details for one element, reimplemented in JSON and XML models
    virtual QVariantMap getVariantMap(QByteArray bytes) = 0;
    //API instance for models for one external API service
    APIBase *apiInstance();

    //Update specific headers on updating
    void updateHeadersData(QNetworkReply *reply);

    //Reset model data
    void reset();

    //Auto generate role names by REST keys
    void generateRoleNames();
    void generateDetailsRoleNames(QVariantMap item);

    //Items management
    RestItem createItem(QVariantMap value);
    void updateItem(QVariantMap value);
    RestItem findItemById(QString id);
    void append(RestItem item);

    QHash<int, QByteArray> roleNames() const;
    QHash<int, QByteArray> detailsRoleNames() const;

protected slots:
    //Properties protected SET methods
    void fetchMoreFinished();
    void fetchDetailFinished();
    void setLoadingStatus(LoadingStatus loadingStatus);
    void setAccept(QString accept);
    void setLoadingErrorString(QString loadingErrorString);
    void setLoadingErrorCode(QNetworkReply::NetworkError loadingErrorCode);

private:
    //Properties store vars
    QHash<int, QByteArray> m_roleNames;
    int m_roleNamesIndex;
    bool m_detailRoleNamesGenerated;
    QList<RestItem> m_items;
    QStringList m_fields;
    QStringList m_expand;
    QString m_idField;
    QStringList m_sort;
    LoadingStatus m_loadingStatus;
    QVariantMap m_filters;
    QString m_loadingErrorString;
    QNetworkReply::NetworkError m_loadingErrorCode;
    QString m_fetchDetailLastId;
    DetailsModel m_detailsModel;
    QQmlPropertyMap m_details;
    Pagination m_pagination;
    APIBase *m_apiInstance;
    bool m_enableDetailsCaching;
};

#endif // BASERESTLISTMODEL_H
