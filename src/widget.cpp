#include "inc/widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    server(new QTcpServer())
{
    ui->setupUi(this);
    setFixedSize(688, 243);

    //按钮实现
    connect(ui->OpenServer, SIGNAL(clicked()), this, SLOT(openServer()));

    connect(ui->CloseServer, SIGNAL(clicked()), this, SLOT(closeServer()));

    ui->CloseServer->setEnabled(false);

    //更新日志时 指针移动到末尾
    connect(ui->LogShow, &QTextBrowser::textChanged, [this] {
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
    if (!server->isListening())
    {
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
    if (server->isListening())
    {
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
    SocketThread* st = new SocketThread(server->nextPendingConnection(),
        &sockets,
        ui->LogShow,
        &db,
        &query
    );
    st->start();
}

void Widget::initDataBase()
{
    //create database
    query.exec("create table users (id int primary key not null, phone int not null, password text not null, friends int)");
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

