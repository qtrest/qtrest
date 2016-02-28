# Qt Micro REST Client Framework

Qt Micro REST Client Framework - small and simple REST API client for any Qt/QML application.
Library support standard JSON and XML REST APIs.

By default library support standard Yii2 REST API. Read more:
- English: https://github.com/yiisoft/yii2/blob/master/docs/guide/rest-quick-start.md
- Russian: https://github.com/yiisoft/yii2/blob/master/docs/guide-ru/rest-quick-start.md

## Features
- Based on QAbstractListModel with fetchMore and canFetchMore support;
- Support JSON and XML list data;
- Pagination by per page, limit/offset or cursor;
- Sorting data;
- Filtering data;
- Authentication;
- StackView navigation;
- Specify fields for GET list method
- Support for Yii2 REST and Django REST Framework from the box;
- Separate model and API methods;
- Simple API implementation for your apps;
- State based model information;
- Requires ID field for each record;
- Interfaces from C++ and Qt Quick/QML;
- Multiple external API services in different models.

## Known issues/TODO
- At now library readonly, without POST/PUT methods;
- Support JSON sub array data, but not support XML subarray data;

## Example application
I will made fully functionality example application, available at here: https://github.com/kafeg/qtrest-example

## Usage
Usage library is simplest as it possible. I will show howto use it on my real example project.

### Include library to your project
First, clone library
```
mkdir PROJECT_ROOT/api/
cd PROJECT_ROOT/api/
git clone https://github.com/kafeg/qtrest.git
```
Then add `include (api/qtrest/qtrest.pri)` to your project file.

### Create your own API class
After setup library we must create class API inherited from existing APIBase, e.g. `api/api.h` and `api/api.cpp`:
``` C++
#ifndef SKIDKZAPI_H
#define SKIDKZAPI_H

#include "apibase.h"
#include "usingleton.h"
#include <QtQml>

class SkidKZApi : public APIBase, public uSingleton<SkidKZApi>
{
    Q_OBJECT
public:
    SkidKZApi();

    //api methods
    //get list of objects
    QNetworkReply *getCoupons(QStringList sort, Pagination *pagination, QVariantMap filters = QVariantMap(), QStringList fields = QStringList());
    //get full data for specified item
    QNetworkReply *getCouponDetail(QString id);
};

#endif // SKIDKZAPI_H
```
And implement this class:
``` C++
#include "skidkzapi.h"
#include <QFile>
#include <QTextStream>
#include <QUrlQuery>

SkidKZApi::SkidKZApi() : APIBase(0), uSingleton<SkidKZApi>(*this)
{
    //Base URL used for all API calls for this example
    setBaseUrl("http://api.skid.kz");

    //You may specify auth token for each API call. And you may write your own api method for authetification
    setAuthToken("Bearer 8aef452ee3b32466209535b96d456b06");
}

//In this methods we get list of objects, based on specified page number, filters, sort and fileds list.
//We can fetch all fields or only needed in our list.
QNetworkReply *SkidKZApi::getCoupons(QStringList sort, Pagination *pagination, 
                                      QVariantMap filters, QStringList fields)
{
    //URL and GET parameters
    QUrl url = QUrl(baseUrl()+"/v1/coupon");
    QUrlQuery query;

    //Specify filters GET param
    if (!sort.isEmpty()) {
        query.addQueryItem("sort", sort.join(","));
    }

    //Specify pagination. We use pagination type from model.
    switch(pagination->policy()) {
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

    //if we need to filter our model, we use filters.
    //Be careful, if you use this methods, your curent pagintaion wil be broken
    //and you must full reaload your model data when you specify new filters
    if (!filters.isEmpty()) {
        QMapIterator<QString, QVariant> i(filters);
        while (i.hasNext()) {
            i.next();
            query.addQueryItem(i.key(), i.value().toString());
        }
    }

    //We may to get all or spicified fields in this method.
    if (!fields.isEmpty()) {
        query.addQueryItem("fields", fields.join(","));
    }

    url.setQuery(query.query());

    QNetworkReply *reply = get(url);

    return reply;
}

//If we fetch e.g. 3 of 10 fields in our 'getCoupons' methods,
//we may to get full information from needed item by it ID
QNetworkReply *SkidKZApi::getCouponDetail(QString id)
{
    if (id.isEmpty()) {
        qDebug() << "ID is empty!";
        return 0;
    }

    QUrl url = QUrl(baseUrl()+"/v1/coupon/"+id);

    QNetworkReply *reply = get(url);

    return reply;
}
```
