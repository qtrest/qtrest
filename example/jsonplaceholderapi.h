#ifndef JSONPLACEHOLDERAPI_H
#define JSONPLACEHOLDERAPI_H

#include <QObject>
#include <QtQml>
#include "apibase.h"

#define URI_GET_POSTS "/posts"

//Sample for https://jsonplaceholder.typicode.com
class JSONPlaceholderApi : public APIBase
{
    Q_OBJECT
public:
    explicit JSONPlaceholderApi(QObject *parent = nullptr);

    //register API object in QML and meta-type system
    static void declareQML()
    {
        qmlRegisterType<JSONPlaceholderApi>("com.github.qtrest.jsonplaceholderapi", 1, 0, "JSONPlaceholderApi");
    }

    //handle all requests from ReadOnly model
    QNetworkReply *handleRequest(QString path,
                                 QStringList sort,
                                 Pagination *pagination,
                                 QVariantMap filters = QVariantMap(),
                                 QStringList fields = QStringList(),
                                 QStringList expand = QStringList(),
                                 QString id = 0);

    //Method API /posts
    QNetworkReply *getPosts(QStringList sort,
                            Pagination *pagination,
                            QVariantMap filters = QVariantMap(),
                            QStringList fields = QStringList(),
                            QStringList expand = QStringList());
};

#endif // JSONPLACEHOLDERAPI_H
