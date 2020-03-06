#ifndef QTREST_RESTITEM_H
#define QTREST_RESTITEM_H

#include <QVariantMap>
#include "qtrest_global.h"

namespace Rest {

class QTREST_EXPORT RestItem {
public:
    RestItem();
    explicit RestItem(QVariantMap object, QString idField);
    QVariant value(QString key);
    QStringList keys();
    QString id() const;
    bool isUpdated();
    bool isValid() const;

    void update (QVariantMap value);

    bool operator==(const RestItem &other);
private:
    QVariantMap m_object;
    QString m_idField;
    bool m_isUpdated;
    bool m_isValid;
};

} // namespace Rest

#endif // QTREST_RESTITEM_H
