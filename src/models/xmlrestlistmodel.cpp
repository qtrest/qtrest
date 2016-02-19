#include "xmlrestlistmodel.h"

#include <QNetworkReply>
#include <QXmlStreamReader>

XmlRestListModel::XmlRestListModel(QObject *parent) : BaseRestListModel(parent)
{
    setAccept("application/xml");
}

QVariantList XmlRestListModel::getVariantList(QByteArray bytes)
{
    QString str = QString::fromUtf8(bytes.data(), bytes.size());

    QXmlStreamReader xml(str);

    QVariantList list;
    QVariantMap map;

    bool isInRoot = false;
    int itemStartCount = 0;
    QString currentElementName;
    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();

        //        TODO At now we has only simple list structure of XML response without subelemets
        //        Need write code to get array of subitems, e.g.
        //
        //        <response>
        //        <item>
        //            <id>1</id>
        //            <sourceServiceId>1</sourceServiceId>
        //            <cityId>1</cityId>
        //            <createTimestamp>2015-03-12 14:01:57</createTimestamp>
        //            <imagesLinks>
        //                <item>https://static.chocolife.me/static/upload/images/deal/for_deal_page/21000/20056/660x305/1_20150312023051426147565.7364.jpg</item>
        //                <item>https://static.chocolife.me/static/upload/images/deal/for_deal_page/21000/20056/660x305/2_20150312023051426147565.9348.jpg</item>
        //                <item>https://static.chocolife.me/static/upload/images/deal/for_deal_page/21000/20056/660x305/4_20150312093171426174997.7985.jpg</item>
        //                <item>https://static.chocolife.me/static/upload/images/deal/for_deal_page/21000/20056/660x305/5_20150312093171426174997.944.jpg</item>
        //                <item>https://www.chocolife.me/</item>
        //            </imagesLinks>
        //            ...
        //        </item>
        //        <item>
        //        ...
        //        </item>
        //
        //        In this example we could not access to imagesLinks because for this we need use recursion or else more. If you need use subarrays, please give me pull request =)

        //we only start and nothing todo
        if (token == QXmlStreamReader::StartDocument)
            continue;

        //each element start
        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name() == rootElement()) {
                isInRoot = true;
                continue;
            }
            if (isInRoot) {
                if (xml.name() == itemElement()) {
                    itemStartCount++;
                }
                if (itemStartCount > 0 && !xml.name().isEmpty()) {
                    currentElementName = xml.name().toString();
                }
            }
        }

        //each element is has text
        if (token == QXmlStreamReader::Characters)
        {
            if (isInRoot) {
                if (itemStartCount > 0) {
                    map.insert(currentElementName, xml.text().toString());
                }
            }
        }

        //each element end
        if (token == QXmlStreamReader::EndElement)
        {
            if (xml.name() == rootElement()) {
                isInRoot = false;
                break;
            }
            if (isInRoot) {
                if (xml.name() == itemElement() && itemStartCount > 0) {
                    itemStartCount--;
                    if (itemStartCount == 0) {
                        list.append(map);
                        map.clear();
                    }
                }
            }
        }
    }

    //qDebug() << list;

    return list;
}

QVariantMap XmlRestListModel::getVariantMap(QByteArray bytes)
{
    QString str = QString::fromUtf8(bytes.data(), bytes.size());

    QXmlStreamReader xml(str);

    QVariantMap map;

    bool isInRoot = false;
    int itemStartCount = 0;
    QString currentElementName;
    while (!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();

        //TODO if you need subarrays please do it yourself and give me pull request ;-)

        //we only start and nothing todo
        if (token == QXmlStreamReader::StartDocument)
            continue;

        //each element start
        if (token == QXmlStreamReader::StartElement)
        {
            if (xml.name() == rootElement()) {
                isInRoot = true;
                continue;
            }
            if (isInRoot) {
                currentElementName = xml.name().toString();
            }
        }

        //each element is has text
        if (token == QXmlStreamReader::Characters)
        {
            if (isInRoot) {
                map.insert(currentElementName, xml.text().toString());
            }
        }

        //each element end
        if (token == QXmlStreamReader::EndElement)
        {
            if (xml.name() == rootElement()) {
                isInRoot = false;
                break;
            }
        }
    }

    return map;
}
