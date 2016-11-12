# Qt / QML REST Client  (Beta)

Qt REST Client  - small and simple REST API client for any Qt/QML application.
Library support standard JSON and XML REST APIs and auto mapping REST data to QAbstractListModel for QML

Manual in russian: https://habrahabr.ru/post/314984

By default library support standard Yii2 REST API and Django REST Framework. Read more:
- English: https://github.com/yiisoft/yii2/blob/master/docs/guide/rest-quick-start.md
- Russian: https://github.com/yiisoft/yii2/blob/master/docs/guide-ru/rest-quick-start.md
- http://www.django-rest-framework.org/

## Features
- Based on QAbstractListModel with fetchMore and canFetchMore support;
- Support JSON and XML list data;
- Pagination by per page, limit/offset or cursor;
- Sorting data;
- Filtering data;
- Authentication;
- StackView navigation;
- Specify fields for GET list method;
- Lazy loading details item data;
- Separate model and API methods;
- Simple API implementation for your apps;
- State based model information;
- Requires ID field for each record;
- Interfaces from C++ and Qt Quick/QML;
- Multiple external API services in different models;
- Predefined or custom model classes

![Qt Micro REST Client Framework](https://raw.githubusercontent.com/kafeg/qtrest/master/docs/restapi.png "REST API Scheme")

## Known issues/TODO
- No cache for data;
- Support JSON subarray data, but not support XML subarray data;

## Example application
I will made fully functionality example application, available at here: https://github.com/kafeg/qtrest-example

## Usage
Usage library is simplest as it possible. I will show howto use it on my real example project.

#### 1. Include library to your project

First, clone library.
```
mkdir PROJECT_ROOT/api/
cd PROJECT_ROOT/api/
git clone https://github.com/kafeg/qtrest.git
```
Then add `include (api/qtrest/com_github_qtrest.pri)` to your project file.

#### 2. Create your own API class
After setup library we must create class API inherited from existing APIBase, e.g. `api/api.h` and `api/api.cpp`:
``` C++
#ifndef SKIDKZAPI_H
#define SKIDKZAPI_H

#include "apibase.h"
#include <QtQml>

class SkidKZApi : public APIBase
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit SkidKZApi();

    //register API object in QML
    static void declareQML() {
        qmlRegisterType<SkidKZApi>("com.github.qtrestexample.skidkzapi", 1, 0, "SkidKZApi");
    }

    //handle all requests from ReadOnly model
    QNetworkReply *handleRequest(QString path, QStringList sort, Pagination *pagination,
                           QVariantMap filters = QVariantMap(), QStringList fields = QStringList(), QString id = 0);

    //Method API /v1/coupon
    QNetworkReply *getCoupons(QStringList sort, Pagination *pagination,
                              QVariantMap filters = QVariantMap(), QStringList fields = QStringList());
    
    //Method API /v1/coupon/{id}
    QNetworkReply *getCouponDetail(QString id);

    //Method API /v1/categories
    QNetworkReply *getCategories(QStringList sort, Pagination *pagination);
};

#endif // SKIDKZAPI_H
```
And implement this class:
``` C++
#include "skidkzapi.h"
#include <QFile>
#include <QTextStream>
#include <QUrlQuery>

SkidKZApi::SkidKZApi() : APIBase(0)
{

}

QNetworkReply *SkidKZApi::handleRequest(QString path, QStringList sort, Pagination *pagination,
                                  QVariantMap filters, QStringList fields, QString id)
{
    if (path == "/v1/coupon") {
        return getCoupons(sort, pagination, filters, fields);
    }
    else if (path == "/v1/coupon/{id}") {
        return getCouponDetail(id);
    }
    else if (path == "/v1/categories") {
        return getCategories(sort, pagination);
    }
}

QNetworkReply *SkidKZApi::getCoupons(QStringList sort, Pagination *pagination, QVariantMap filters, QStringList fields)
{
    QUrl url = QUrl(baseUrl()+"/v1/coupon");
    QUrlQuery query;

    //Sorting
    if (!sort.isEmpty()) {
        query.addQueryItem("sort", sort.join(","));
    }

    //Paging
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

    //Filter
    if (!filters.isEmpty()) {
        QMapIterator<QString, QVariant> i(filters);
        while (i.hasNext()) {
            i.next();
            query.addQueryItem(i.key(), i.value().toString());
        }
    }

    //Only needed fields
    if (!fields.isEmpty()) {
        query.addQueryItem("fields", fields.join(","));
    }

    //Make query
    url.setQuery(query.query());

    //Make GET request
    QNetworkReply *reply = get(url);

    return reply;
}

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

QNetworkReply *SkidKZApi::getCategories(QStringList sort, Pagination *pagination)
{
    QUrl url = QUrl(baseUrl()+"/v1/categories");
    QUrlQuery query;

    if (!sort.isEmpty()) {
        query.addQueryItem("sort", sort.join(","));
    }

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

    url.setQuery(query.query());

    QNetworkReply *reply = get(url);

    return reply;
}
```

#### 3. Create your model classes or use completed components, based on your API

##### 3.1 Completed components

For simple readonly models you may use complete QML data components called JsonRestListModel {} and XmlRestListModel {}. You hasn't access to manipulate data in this models and pre process items, but by using it you may to skip defining custom C++ models.

For example, start in your QML code:

```
...

SkidKZApi {
    id: skidKZApi
    ...
}

JsonRestListModel {
    id: jsonCouponsModel
    api: skidKZApi

    idField: 'id'

    requests {
        get: "/v1/coupon"
        getDetails: "/v1/coupon/{id}"
    }

    filters: {'isArchive': '0'}
    fields: ['id','title']
    sort: ['-id']

    pagination {
        policy: Pagination.PageNumber
        perPage: 20
    }

    Component.onCompleted: { reload(); }
}
```

##### 3.2 Custom models
For example we create one model, but you may use one API class for multiple models. E.g. you may use one API class for get list of coupons and for list of categories.

At this point you have full control on your rest data.

You model class must reimplement 6 methods:
```
- declareQML(); //Declare you model for use it in QML code
- fetchMoreImpl(); //Call API fucntion for fetchMore function (e.g. getCoupons())
- fetchDetailImpl(); //Call API fucntion for fetchDetails for one item (e.g. getCouponDetail())
- preProcessItem(); //Pre proccess each new list item for manage field list
```

For example we make Coupons model from our example app (api/models/couponmodel.h):
``` C++
#ifndef COUPONMODEL_H
#define COUPONMODEL_H

//We use JSON API version
#include "jsonrestlistmodel.h"

//We use our Skid.KZ API
#include "api/skidkzapi.h"

class CouponModel : public JsonRestListModel
{
    Q_OBJECT
public:
    explicit CouponModel(QObject *parent = 0);

    static void declareQML() {
        JsonRestListModel::declareQML();
        qmlRegisterType<CouponModel>("com.github.qtrest.coupons", 1, 0, "CouponModel");
    }
protected:
    QNetworkReply *fetchMoreImpl(const QModelIndex &parent);
    QNetworkReply *fetchDetailImpl(QString id);
    QVariantMap preProcessItem(QVariantMap item);
};

#endif // COUPONMODEL_H
```
And implement this class:
``` C++
#include "couponmodel.h"

CouponModel::CouponModel(QObject *parent) : JsonRestListModel(parent)
{

}

QNetworkReply *CouponModel::fetchMoreImpl(const QModelIndex &parent)
{
    Q_UNUSED(parent)

    return static_cast<SkidKZApi *>(apiInstance())->getCoupons(sort(), pagination(), filters(), fields());
}

QNetworkReply *CouponModel::fetchDetailImpl(QString id)
{
    return static_cast<SkidKZApi *>(apiInstance())->getCouponDetail(id);
}


//Data management and preparation is function of Backend developer, but if he or she 
//is do not want to deal with data preparation, you may to prepare each item yourself
QVariantMap CouponModel::preProcessItem(QVariantMap item)
{
    QDate date = QDateTime::fromString(item.value("createTimestamp").toString(), 
                                        "yyyy-MM-dd hh:mm:ss").date();
    item.insert("createDate", date.toString("dd.MM.yyyy"));

    ...

    return item;
}
```
At this point we alredy full implemented our API and model. For use it from C++ you may use this model as is.
For use it from QML you must to add some code to `main.cpp`:
``` С++
#include "api/models/couponmodel.h"
...
int main(int argc, char *argv[])
{
    ...
    //api and models
    SkidKZApi::declareQML();
    CouponModel::declareQML();
    ...
}
```

#### 4. Use model from QML
At first, we must declare our model:
``` QML
import com.github.qtrestexample.skidkzapi 1.0
import com.github.qtrest.coupons 1.0
import com.github.qtrest.pagination 1.0

...

SkidKZApi {
    id: skidKZApi

    baseUrl: "http://api.skid.kz"

    authTokenHeader: "Authorization"
    authToken: "Bearer 8aef452ee3b32466209535b96d456b06"

    Component.onCompleted: console.log("completed!");
}

...
CouponModel {
        id: coupons;
        api: skidKZApi
        
        //Note: only if our APi support filtering
        //Specify base filter, than we make filters form and set filters dynamicaly
        filters: {'isArchive': '0'}
        
        //we must cpecify ID field for correct interaction with API
        idField: 'id'
        
        //Note: only if our APi support fields
        //In ListView we need only base fileds, and exclude longDescription fields and other.
        fields: ['id','title','sourceServiceId','imagesLinks',
                'mainImageLink','pageLink','cityId','boughtCount','shortDescription',
                'createTimestamp', 'serviceName', 'discountType', 'originalCouponPrice', 
                'originalPrice', 'discountPercent', 'discountPrice']
        
        //Note: only if our APi support sorting
        //Additional param for sorting our results
        sort: ['-id']

        //If we need to paging our API, we need to specify paging method. Default is None.
        pagination {
            policy: Pagination.PageNumber
            perPage: 20
            currentPageHeader: "X-Pagination-Current-Page"
            totalCountHeader: "X-Pagination-Total-Count"
            pageCountHeader: "X-Pagination-Page-Count"
        }

        //And when model is complete we call reload function for load elements
        Component.onCompleted: { reload(); }
    }
```

Then we can use the model as a standard element:
``` QML
ListView {
    id: couponsList
    model: coupons
    
    property string detailSource: "qrc:/CouponDetail.qml"
    
    delegate: Item {
        Text {
            text: title
        }
        
        MouseArea {
            id: detail
            anchors.fill: parent
    
            onClicked: {
                couponsModel.fetchDetail(id)
                stackView.push(couponsList.detailSource, 
                {detailsModel: couponsModel.detailsModel, 
                couponsModel: couponsModel})
            }
        }
    }
}
```

## Advanced usage

#### 1. DetailsView page
Also, we have full support for StackView navigation by special 'details model' available in each your model, based on QSortFilterModel and using 'ID' field as filter. For example, we have ListView in one Stack element, and DrtailView in other stack element.
We may fetch details info for one of elements and send this element into Details page, when we may use simple hack for display one element with detail info:
``` QML
import QtQuick 2.6
import Qt.labs.controls 1.0
import com.github.qtrest.coupons 1.0

Item {
    id: details
    anchors.fill: parent

    property string titleText: qsTr("Detail")

    property var detailsModel
    property var couponsModel
    property var loadingStatus: couponsModel.loadingStatus

    onLoadingStatusChanged: {
        if (loadingStatus == CouponModel.IdleDetails) {
            pageLoader.sourceComponent = detailComponent
        }
    }

    Loader {
        id: pageLoader
    }

    MouseArea {
        anchors.fill: parent
        onClicked: pageLoader.sourceComponent = detailComponent
    }

    BusyIndicator {
        id: loadingIndicator
        width: settings.busyIndicatorSize*1.5
        height: settings.busyIndicatorSize*1.5

        running: loadingStatus == CouponModel.LoadDetailsProcessing
        visible: opacity > 0
        opacity: loadingStatus == CouponModel.LoadDetailsProcessing ? 1 : 0
        anchors.centerIn: parent
        Behavior on opacity {
            NumberAnimation { duration: 400; }
        }
    }

    Component {
        id: detailComponent

        ListView {
            id: couponsList
            width: details.width
            height: details.height
            model: detailsModel
            interactive: false

            delegate: ItemDelegate {
                id: delegate
                width: couponsList.width;
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
```
In this code we open Detail page, waitng for loading details info with BusyIndicator displaying, and after loading complete - display full information for item. Our hack is in ListView, it will be not interactive and display only one item.
