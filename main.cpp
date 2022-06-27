#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTcpSocket>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/watcher/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QTcpSocket socket;
    socket.connectToHost("192.168.1.100", 50001);

    socket.write("GET / HTTP/1.0\r\n\r\n");
    while (socket.waitForReadyRead())
        qDebug() << socket.readAll().data();

    return app.exec();
}
