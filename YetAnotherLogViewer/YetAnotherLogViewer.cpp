// YetAnotherLogViewer.cpp : Defines the entry point for the application.

#define DEBUG 1

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "YetAnotherLogViewer.hpp"
#include "LoggerModel.hpp"



int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    qmlRegisterType<LoggerModel>("LoggerModel", 1, 0, "LoggerModel");
#if DEBUG
    qDebug() << "creating a logger Model object";
#endif
    LoggerModel* loggerModel = new LoggerModel();
    engine.rootContext()->setContextProperty(QStringLiteral("loggerModel"), loggerModel);
    const QUrl url(QStringLiteral("qrc:/YetAnotherLogViewer/Main.qml"));
    engine.load(url);
    return app.exec();
}