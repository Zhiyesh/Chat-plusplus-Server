#include "inc/widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    server(new QTcpServer()),
    socket(new QTcpSocket())
{
    ui->setupUi(this);
    setFixedSize(688, 243);

    //按钮实现
    connect(ui->OpenServer, SIGNAL(clicked()), this, SLOT(openServer()));

    connect(ui->CloseServer, SIGNAL(clicked()), this, SLOT(closeServer()));

    ui->CloseServer->setEnabled(false);

    //不错过最新日志
    connect(ui->LogShow, &QTextBrowser::textChanged, [&](){
        ui->LogShow->moveCursor(QTextCursor::End);
    });

    //Database init
    db = QSqlDatabase::addDatabase(USE_DATABASE);
    db.setDatabaseName(DATABASE_PATH);
    if (db.open()) {
        query = QSqlQuery(db);
    }
    initDataBase();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::openServer()
{
    if (!server->isListening()) {
        ui->LogShow->append("正在打开...");
        Zy::mSleep(600);
        server->listen(ADDRESS, PORT);
        connect(server, &QTcpServer::newConnection, this, &Widget::newConnectionHandle);
        QString log = QString(QDateTime::currentDateTime().toString(TIME_FORMAT)).append("\t服务开启成功!");
        ui->OpenServer->setEnabled(false);
        ui->CloseServer->setEnabled(true);
        ui->LogShow->append(log);
    }
}

bool Widget::closeServer()
{
    if (server->isListening()) {
        ui->LogShow->append("正在关闭...");
        Zy::mSleep(600);
        server->disconnect();
        server->close();
        QString log = QString(QDateTime::currentDateTime().toString(TIME_FORMAT))
                .append("\t服务已关闭。");
        ui->OpenServer->setEnabled(true);
        ui->CloseServer->setEnabled(false);
        ui->LogShow->append(log);
    }
    return true;
}

void Widget::newConnectionHandle()
{
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, [&](){
        Zy::mSleep(300);
        handleMsg(socket->readAll());
    });
}

void Widget::handleMsg(QString msg)
{
#define SOCKET_WRITE(data) {            \
        socket->write(data);            \
        socket->waitForBytesWritten();  \
    }

    QStringList cmd = msg.split("#");

    if (cmd[0] == "login") {
        qDebug() << "接收到login信号";
        if (checkFromDB(cmd[1], cmd[2])) {
            SOCKET_WRITE("00000001");
        }
        else SOCKET_WRITE("00000000");
    }
    else if (cmd[0] == "logining") {
        qDebug() << "接收到logining信号";
        ui->LogShow->append(QString("%1\t账号%2已登录")
                            .arg(QDateTime::currentDateTime().toString(TIME_FORMAT))
                            .arg(cmd[1]));
        sockets.insert(cmd[1], socket);
    }
    else if (cmd[0] == "exit") {
        qDebug() << "接收到exit信号";
        ui->LogShow->append(QString("%1\t账号%2已退出")
                            .arg(QDateTime::currentDateTime().toString(TIME_FORMAT))
                            .arg(cmd[1]));
        sockets.remove(cmd[1]);
    }
    else if (cmd[0] == "send") {
        qDebug() << "接收到send信号";
        //设置为接收人的socket
        socket = sockets.value(cmd[2]);
        if (socket != nullptr) {
            //发送标志
            SOCKET_WRITE("msg#");
            //发送发送人
            SOCKET_WRITE(QString("%1#").arg(cmd[1]).toStdString().c_str());
            //发送内容
            SOCKET_WRITE(QString(cmd[3]).toStdString().c_str());
        }
    }
    else if (cmd[0] == "getmyfri") {
        qDebug() << "接收到getmyfri信号";
        //设置为接收人的socket
        socket = sockets.value(cmd[1]);
        QString friend_list;
        if (socket != nullptr) {
            //发送好友
            SOCKET_WRITE("recmyfri#");
            query.exec(QString("select * from users where phone = %1").arg(cmd[1]));
            while (query.next()) {
                friend_list.append(query.value("friends").toString());
                friend_list.append("z");
            }
            SOCKET_WRITE(friend_list.toStdString().c_str());
        }
    }
}

void Widget::initDataBase()
{
    //create database
    query.exec("create table users (id int primary key not null, phone int not null, password text not null, friends int)");
}

bool Widget::checkFromDB(QString phone, QString password)
{
    query.exec(QString("select * from users where phone = %1").arg(phone));

    while (query.next()) {
        if (query.value("phone").toString() == "") continue;
        if (query.value("password").toString() == password) {
            return true;
        }
    }
    return false;
}

void Widget::closeEvent(QCloseEvent *event)
{
    if (this->closeServer()) {
        db.close();
        //正常退出
        event->accept();
    }
    else {
        //不退出
        event->ignore();
        ui->LogShow->append("服务还在运行");
    }
}

