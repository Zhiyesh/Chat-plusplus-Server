#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextBrowser>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "inc/sleep.h"

#define TIME_FORMAT  "MM.dd.yyyy hh:mm:ss"

#define SOCKET_WRITE(X) do { \
    socket->write(QString(X).toUtf8().data());  \
    socket->waitForBytesWritten();              \
} while (0)

class SocketThread : public QThread
{
    Q_OBJECT
public:
    explicit SocketThread(QObject *parent = 0);
    SocketThread(QTcpSocket* socket,
                 QMap<QString, QTcpSocket*>* sockets,
                 QTextBrowser* LogShow,
                 QSqlDatabase* db,
                 QSqlQuery* query);

protected:
    void run() override
    {
        connect(socket, &QTcpSocket::readyRead, [this] {
            Zy::mSleep(300);
            handleMsg(socket->readAll());
        });
    }

protected:
    void handleMsg(QString msg);
    void sendMessage(QStringList cmd);
    bool checkFromDB(QString phone, QString password);

private:
    QTcpSocket* socket;
    QTextBrowser* LogShow;
    QMap<QString, QTcpSocket*>* sockets;
    QSqlDatabase* db;
    QSqlQuery* query;
};

#endif // SOCKETTHREAD_H
