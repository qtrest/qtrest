import QtQuick 2.0
import QtQuick.Controls 2.0

import com.github.qtrest.jsonplaceholderapi 1.0
import com.github.qtrest.pagination 1.0
import com.github.qtrest.requests 1.0
import com.github.qtrest.jsonrestlistmodel 1.0

ApplicationWindow
{
    visible: true
    width: 640
    height: 480
    title: qsTr("Minimal Qml")

    JSONPlaceholderApi
    {
        id: jsonApi

        baseUrl: "http://api.skid.kz"

        //authTokenHeader: "Authorization"
        //authToken: "Bearer 8aef452ee3b32466209535b96d456b06"

        Component.onCompleted: console.log("completed!");
    }

    JsonRestListModel
    {
        id: jsonModel
        api: jsonApi

        //we must cpecify ID field for correct interaction with API
        idField: 'id'

        //Note: only if our APi support fields
        //In ListView we need only base fields, and exclude longDescription fields and other.
        fields: ['id','userId','title','body']

        //Note: only if our APi support sorting
        //Additional param for sorting our results
        sort: ['-id']

        //And when model is complete we call reload function for load elements
        Component.onCompleted: { reload(); }
    }

    ListView {
        id: jsonList
        model: jsonModel

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
}
