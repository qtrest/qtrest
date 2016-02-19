#include "jsonrestlistmodel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonDocument>

JsonRestListModel::JsonRestListModel(QObject *parent) : BaseRestListModel(parent)
{
    setAccept("application/json");
}

QVariantList JsonRestListModel::getVariantList(QByteArray bytes)
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

QVariantMap JsonRestListModel::getVariantMap(QByteArray bytes)
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
