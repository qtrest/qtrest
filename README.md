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

## Usage
Usage library is simplest as it possible. I will show howto use it on real example.

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
#ifndef API_H
#define API_H

#include "apibase.h"
#include "usingleton.h"
#include <QtQml>

/*
  Specify singleton for each API
  In one application you may use multiple external API services
  for different models, but each API class must be a singleton
  because one external service must be managed by one Api class.
*/
#define restapi API::instance()

class API : public APIBase, public uSingleton<API>
{
    Q_OBJECT
public:
    API();

    //define GET list method for load list data
    QNetworkReply *getCoupons(QStringList sort, Pagination *pagination, 
                              QVariantMap filters = QVariantMap(), 
                              QStringList fields = QStringList());
    
    //define GET detaiils (full fields) for specified item
    QNetworkReply *getCouponDetail(QString id);
};

#endif // API_H
```
