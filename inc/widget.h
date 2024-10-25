#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QMap>
#include <QDateTime>
#include <QCloseEvent>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "inc/listenip.h"
#include "inc/sleep.h"
#include "inc/socketthread.h"

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
    void initDataBase();
    void closeEvent(QCloseEvent *event);

private:
    Ui::Widget *ui;

    QMap<QString, QTcpSocket*> sockets;
    QSqlQuery query;
    QTcpServer *server;
    QSqlDatabase db;

    friend class SocketThread;
};

#endif // WIDGET_H
