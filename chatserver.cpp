#include "chatserver.h"

#include <QTcpServer>
#include <QHostAddress>
#include <QTcpSocket>

ChatServer::ChatServer(const QHostAddress &address, int port, QObject *parent) : QObject(parent)
{
    QTcpServer *server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), SLOT(onNewConnection()));
    server->listen(address, port);
}

ChatServer::~ChatServer()
{

}

void ChatServer::onNewConnection()
{
    QTcpServer *server = (QTcpServer *) sender();
    QTcpSocket *socket = server->nextPendingConnection();
    socket->write("Hi!\n");
}
