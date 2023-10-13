#include <QCoreApplication>
#include "jsonplaceholderapi.h"
#include "jsonrestlistmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

#ifdef WITH_QML_SUPPORT
    JSONPlaceholderApi::declareQML();
    JsonRestListModel::declareQML();

    // please check 'main.qml' for more details - this uses Api and model and attach to the QML GUI
    // instances of API and Models - created directly inside QML because of calls of the 'declareQML()'
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
#else

    QTimer::singleShot(100, [](){
        // API class, can be used with Qt Models
        qDebug() << "Prepare API";
        JSONPlaceholderApi jsonPlaceHolderApi;

        // comment line below to get error output
        jsonPlaceHolderApi.setBaseUrl("https://jsonplaceholder.typicode.com");

        // Qt model sample which uses Api class
        // usually Qt models can be used with QtWidgets or QML
        // but here special sample just without GUI dependencies to show how it works
        qDebug() << "Prepare model";
        JsonRestListModel jsonRestListModel;
        jsonRestListModel.setApiInstance(&jsonPlaceHolderApi);
        jsonRestListModel.requests()->setGet("/posts");
        jsonRestListModel.requests()->setGetDetails("/posts/{id}");

        qDebug() << "Fetching data";
        QEventLoop el;
        jsonRestListModel.reload();
        QObject::connect(&jsonRestListModel, &JsonRestListModel::countChanged, [&el, &jsonRestListModel](){
            qDebug() << "Data fetched" << jsonRestListModel.count();
            el.quit();
        });

        QTimer::singleShot(5000, [&el](){ el.quit(); });
        el.exec();
        qApp->quit();
    });
#endif

    return app.exec();
}
