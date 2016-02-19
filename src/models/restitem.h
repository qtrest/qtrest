#ifndef RESTITEM_H
#define RESTITEM_H

#include <QVariantMap>

class RestItem {
public:
    explicit RestItem(QVariantMap object, QString idField);
    QVariant value(QString key);
    QStringList keys();
    QString id() const;
    bool isUpdated();

    void update (QVariantMap value);

    bool operator==(const RestItem &other);
private:
    QVariantMap m_object;
    QString m_idField;
    bool m_isUpdated;
};

#endif // RESTITEM_H
