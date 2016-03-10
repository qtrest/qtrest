# Qt Micro REST Client Framework (Beta)

Qt Micro REST Client Framework - small and simple REST API client for any Qt/QML application.
Library support standard JSON and XML REST APIs.

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
- Multiple external API services in different models.

![Qt Micro REST Client Framework](https://raw.githubusercontent.com/kafeg/qtrest/master/docs/restapi.png "REST API Scheme")

## Known issues/TODO
- At now library readonly, without POST/PUT methods;
- Support JSON subarray data, but not support XML subarray data;

## Example application
I will made fully functionality example application, available at here: https://github.com/kafeg/qtrest-example

## Usage
Usage library is simplest as it possible. I will show howto use it on my real example project.

#### 1. Include library to your project

You may use qpm(https://github.com/Cutehacks/qpm):
qpm install com.github.qtrest

Or you may include library manualy. First, clone library
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
#include "usingleton.h"
#include <QtQml>

class SkidKZApi : public APIBase, public uSingleton<SkidKZApi>
{
    Q_OBJECT
public:
    SkidKZApi();

    //api methods
    //get list of objects
    QNetworkReply *getCoupons(QStringList sort, Pagination *pagination, 
                                QVariantMap filters = QVariantMap(), 
                                QStringList fields = QStringList());
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

#### 3. Create your model classes, based on your API
For example we create one model, but you may use one API class for multiple models. E.g. you may use one API class for get list of coupons and for list of categories.

You model class must reimplement 6 methods:
```
- declareQML(); //Declare you model for use it in QML code
- fetchMoreImpl(); //Call API fucntion for fetchMore function (e.g. getCoupons())
- fetchDetailImpl(); //Call API fucntion for fetchDetails for one item (e.g. getCouponDetail())
- preProcessItem(); //Pre proccess each new list item for manage field list
- apiInstance(); //Make your API implementation available for base classes
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

    APIBase *apiInstance();
};

#endif // COUPONMODEL_H
```
And implement this class:
``` C++
#include "couponmodel.h"

CouponModel::CouponModel(QObject *parent) : JsonRestListModel(parent)
{

}

APIBase *CouponModel::apiInstance()
{
    return &SkidKZApi::instance();
}

QNetworkReply *CouponModel::fetchMoreImpl(const QModelIndex &parent)
{
    return SkidKZApi::instance().getCoupons(sort(), pagination(), filters(), fields());
}

QNetworkReply *CouponModel::fetchDetailImpl(QString id)
{
    return SkidKZApi::instance().getCouponDetail(id);
}

//Data management and preparation is function of Backend developer, but if he or she 
//is do not want to deal with data preparation, you may to prepare each item yourself
QVariantMap CouponModel::preProcessItem(QVariantMap item)
{
    QDate date = QDateTime::fromString(item.value("createTimestamp").toString(), 
                                        "yyyy-MM-dd hh:mm:ss").date();
    item.insert("createDate", date.toString("dd.MM.yyyy"));

    QString originalCouponPrice = item.value("originalCouponPrice").toString().trimmed();
    if (originalCouponPrice.isEmpty()) { originalCouponPrice = "?"; }
    QString discountPercent = item.value("discountPercent").toString()
    .trimmed().remove("—").remove("-").remove("%");
    if (discountPercent.isEmpty()) { discountPercent = "?"; }
    QString originalPrice = item.value("originalPrice").toString().trimmed();
    if (originalPrice.isEmpty()) { originalPrice = "?"; }
    QString discountPrice = item.value("discountPrice").toString().remove("тг.").trimmed();
    if (discountPrice.isEmpty()) { discountPrice = "?"; }

    QString discountType = item.value("discountType").toString();
    QString discountString = tr("Undefined Type");
    if (discountType == "freeCoupon" || discountType == "coupon") {
        discountString = tr("Coupon: %1. Discount: %2%")
        .arg(originalCouponPrice).arg(discountPercent);
    } else if (discountType == "full") {
        discountString = tr("Cost: %1. Certificate: %2. Discount: %3%")
        .arg(originalPrice).arg(discountPrice).arg(discountPercent);
    }

    item.insert("discountString", discountString);

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
    //models
    CouponModel::declareQML();
    ...
}
```

#### 4. Use model from QML
At first, we must declare our model:
``` QML
import com.github.qtrest.coupons 1.0
import com.github.qtrest.pagination 1.0

...

CouponModel {
        id: coupons;
        
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

## Additional usage

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
