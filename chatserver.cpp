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
    connect(socket, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

void ChatServer::onReadyRead()
{
    QTcpSocket *socket = (QTcpSocket *) sender();
    while (socket->canReadLine()) {
        parseLine(socket, QString::fromUtf8(socket->readLine()).trimmed());
    }
}

void ChatServer::parseLine(QTcpSocket *sender, const QString &line)
{
    Peer &peer = peers[sender];
    qDebug() << peer.name << line;

    if (line.startsWith("/join ")) {
        // disconnect from current room
        if (!peer.room.isEmpty()) {
            rooms[peer.room].removeAll(sender);
        }

        peer.room = line.mid(6).trimmed();
        rooms[peer.room].append(sender);
    } else if (line.startsWith("/nick ")) {
        peer.name = line.mid(6).trimmed();
    } else if (!peer.name.isEmpty() && !peer.room.isEmpty()) {
        foreach (QTcpSocket *socket, rooms[peer.room]) {
            if (socket != sender) {
                socket->write(QString("%1> %2\n").arg(peer.name).arg(line).toUtf8());
            }
        }
    } else {
        sender->write("Join a room and pick a name first.\n");
    }
}
