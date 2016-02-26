#include "restitem.h"
#include <QDebug>

RestItem::RestItem(QVariantMap object, QString idField) {
    m_object = object;
    m_idField = idField;
    m_isUpdated = false;
}

QVariant RestItem::value(QString key) {
    //qDebug() << key << m_object.value(key);
    return m_object.value(key);
}

QStringList RestItem::keys() {
    return m_object.keys();
}

QString RestItem::id() const {
    return m_object.value(m_idField).toString();
}

bool RestItem::isUpdated() {
    return m_isUpdated;
}

void RestItem::update(QVariantMap value) {
    QMapIterator<QString, QVariant> i(value);
    while (i.hasNext()) {
        i.next();
        //qDebug() << "INSERT" << i.key() << "NEW" << i.value() << "OLD" << m_object.value(i.key());
        m_object.insert(i.key(), i.value());
        //qDebug() << "INSERTED" << m_object.value(i.key());
    }
    m_isUpdated = true;
}

bool RestItem::operator==(const RestItem &other) {
    return id() == other.id();
}
