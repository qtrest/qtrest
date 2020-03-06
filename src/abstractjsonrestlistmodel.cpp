#include "abstractjsonrestlistmodel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonDocument>

using namespace Rest;

AbstractJsonRestListModel::AbstractJsonRestListModel(QObject *parent) : BaseRestListModel(parent)
{
    setAcceptHeaderValue("application/json");
}

QVariantList AbstractJsonRestListModel::getVariantList(QByteArray bytes)
{
    QString str = QString::fromUtf8(bytes.data(), bytes.size());

    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &parseError);
    QJsonArray jsonArray = document.array();

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << parseError.errorString();
    }

    return jsonArray.toVariantList();
}

QVariantMap AbstractJsonRestListModel::getVariantMap(QByteArray bytes)
{
    QString str = QString::fromUtf8(bytes.data(), bytes.size());

    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &parseError);
    QJsonObject obj = document.object();

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << parseError.errorString();
    }

    return obj.toVariantMap();
}
