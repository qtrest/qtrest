#include <QApplication>
#include "jsonplaceholderapi.h"
#include "jsonrestlistmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    JSONPlaceholderApi::declareQML();
    JsonRestListModel::declareQML();

//    JSONPlaceholderApi jsonPlaceHolderApi;

//    JsonRestListModel jsonRestListModel;
//    jsonRestListModel.setApiInstance(&jsonPlaceHolderApi);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
