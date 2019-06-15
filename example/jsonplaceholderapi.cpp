#include "jsonplaceholderapi.h"

JSONPlaceholderApi::JSONPlaceholderApi(QObject *parent)
    : APIBase(parent)
{

}

QNetworkReply *JSONPlaceholderApi::handleRequest(QString path, QStringList sort, Pagination *pagination, QVariantMap filters, QStringList fields, QStringList expand, QString id)
{
    if (path == URI_GET_POSTS) {
        return getPosts(sort, pagination, filters, fields, expand);
    }
}

QNetworkReply *JSONPlaceholderApi::getPosts(QStringList sort, Pagination *pagination, QVariantMap filters, QStringList fields, QStringList expand)
{
    QUrl url = QUrl(baseUrl() + URI_GET_POSTS);
    QUrlQuery query;

    //Sorting
    if (!sort.isEmpty())
    {
        query.addQueryItem("sort", sort.join(","));
    }

    //Paging
    switch(pagination->policy())
    {
    case Pagination::PageNumber:
        query.addQueryItem("per-page", QString::number(pagination->perPage()));
        query.addQueryItem("page", QString::number(pagination->currentPage()));
        break;
    case Pagination::None:
    case Pagination::Infinity:
    case Pagination::LimitOffset:
    case Pagination::Cursor:
    default:
        break;
    }

    //Filter
    if (!filters.isEmpty())
    {
        QMapIterator<QString, QVariant> i(filters);
        while (i.hasNext())
        {
            i.next();
            query.addQueryItem(i.key(), i.value().toString());
        }
    }

    //Only needed fields
    if (!fields.isEmpty())
    {
        query.addQueryItem("fields", fields.join(","));
    }

    //Additional fields to expand relational data
    if (!expand.isEmpty())
    {
        query.addQueryItem("expand", expand.join(","));
    }

    //Make query
    url.setQuery(query.query());

    //Make GET request
    QNetworkReply *reply = get(url);

    return reply;
}
