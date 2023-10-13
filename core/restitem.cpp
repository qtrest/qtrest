#include "restitem.h"
#include <QDebug>

RestItem::RestItem() : m_isUpdated(false), m_isValid(false) {
}

RestItem::RestItem(QVariantMap object, QString idField) {
    m_object = object;
    m_idField = idField;
    m_isUpdated = false;
    m_isValid = true;
}

QVariant RestItem::value(QString key) {
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

bool RestItem::isValid() const
{
    return m_isValid;
}

void RestItem::update(QVariantMap value) {
    QMapIterator<QString, QVariant> i(value);
    while (i.hasNext()) {
        i.next();
        m_object.insert(i.key(), i.value());
    }
    m_isUpdated = true;
}

bool RestItem::operator==(const RestItem &other) {
    return id() == other.id();
}
