#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QMap>
#include <QObject>

class QHostAddress;
class QTcpSocket;

struct Peer
{
    QString name;
    QString room;
};

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(const QHostAddress &address, int port, QObject *parent = 0);
    ~ChatServer();

signals:
    void systemMessage(const QString &s);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QString toString(QTcpSocket *socket);
    void parseLine(QTcpSocket *sender, const QString &line);
    typedef QList<QTcpSocket *> SocketList;
    QMap<QTcpSocket *, Peer> peers;
    QMap<QString, SocketList> rooms;
};

#endif // CHATSERVER_H
