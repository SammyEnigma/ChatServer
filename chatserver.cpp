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
    emit systemMessage(tr("New connection from %1.").arg(toString(socket)));
    connect(socket, SIGNAL(readyRead()), SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), SLOT(onDisconnected()));
}

void ChatServer::onReadyRead()
{
    QTcpSocket *socket = (QTcpSocket *) sender();
    while (socket->canReadLine()) {
        parseLine(socket, QString::fromUtf8(socket->readLine()).trimmed());
    }
}

void ChatServer::onDisconnected()
{
    QTcpSocket *socket = (QTcpSocket *) sender();
    emit systemMessage(tr("%1 has disconnected.").arg(toString(socket)));
}

QString ChatServer::toString(QTcpSocket *socket)
{
    if (!peers[socket].name.isEmpty()) {
        return QString("%3 (%1:%2)").arg(socket->peerAddress().toString()).arg(socket->peerPort()).arg(peers[socket].name);
    } else {
        return QString("%1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort());
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
        systemMessage(tr("%1 has joined %2.").arg(toString(sender)).arg(peer.room));
    } else if (line.startsWith("/nick ")) {
        systemMessage(tr("%1 is now known as %2.").arg(toString(sender)).arg(line.mid(6).trimmed()));
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
