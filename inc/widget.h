#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "inc/listenip.h"
#include "inc/sleep.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QDateTime>
#include <QCloseEvent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

#define TIME_FORMAT  "MM.dd.yyyy hh:mm:ss"

#define USE_DATABASE "QSQLITE"
#define DATABASE_PATH "./ChatppServer.db"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void openServer();
    bool closeServer();
    void newConnectionHandle();
    void handleMsg(QString msg);
    void initDataBase();
    bool checkFromDB(QString phone, QString password);
    void closeEvent(QCloseEvent *event);

private:
    Ui::Widget *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    QMap<QString, QTcpSocket*> sockets;
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // WIDGET_H
