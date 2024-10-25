#include "inc/socketthread.h"

SocketThread::SocketThread(QObject *parent) : QThread(parent)
{
}

SocketThread::SocketThread(QTcpSocket* socket,
                           QMap<QString, QTcpSocket*>* sockets,
                           QTextBrowser* LogShow, QSqlDatabase* db,
                           QSqlQuery* query) :
    socket(socket),
    LogShow(LogShow),
    sockets(sockets),
    db(db),
    query(query)
{

}

void SocketThread::handleMsg(QString msg)
{
//    qDebug() << sockets.keys();
//    for (auto i = sockets.begin(); i != sockets.end(); ++i) {
//        qDebug() << i.key();
//        qDebug() << i.value();
//    }

    QStringList cmd = msg.split("#");

    if (cmd[0] == "login") {
        if (checkFromDB(cmd[1], cmd[2])) {
            SOCKET_WRITE("00000001");
        }
        else SOCKET_WRITE("00000000");
        return;
    }
    if (cmd[0] == "logining") {
        LogShow->append(QString("%1\t账号%2已登录")
                            .arg(QDateTime::currentDateTime().toString(TIME_FORMAT))
                            .arg(cmd[1]));
        sockets->insert(cmd[1], socket);
        return;
    }
    if (cmd[0] == "exit") {
        LogShow->append(QString("%1\t账号%2已退出")
                            .arg(QDateTime::currentDateTime().toString(TIME_FORMAT))
                            .arg(cmd[1]));
        sockets->remove(cmd[1]);
        return;
    }
    if (cmd[0] == "send") {
        sendMessage(cmd);
        return;
    }
    if (cmd[0] == "getmyfri") {
        QString friend_list;
        //发送好友
        SOCKET_WRITE("recmyfri#");
        query->exec(QString("select * from users where phone = %1").arg(cmd[1]));
        while (query->next()) {
            friend_list.append(query->value("friends").toString());
            friend_list.append("z");
        }
        SOCKET_WRITE(friend_list.toUtf8().data());
        return;
    }
}

void SocketThread::sendMessage(QStringList cmd)
{
    if (cmd[0] == "send") {
        QTcpSocket* self_socket = socket;
        socket = sockets->value(cmd[2]);

        if (socket == nullptr) return;

        //发送信息
        socket->write(QString("msg#%1#%2").arg(cmd[1]).arg(cmd[3]).toUtf8().data());

        socket = self_socket;
    }
}

bool SocketThread::checkFromDB(QString phone, QString password)
{
    query->exec(QString("select * from users where phone = %1").arg(phone));

    while (query->next()) {
        if (query->value("phone").toString() == "") continue;
        if (query->value("password").toString() == password) {
            return true;
        }
    }
    return false;
}

