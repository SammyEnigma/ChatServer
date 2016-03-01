#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>

class QHostAddress;

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(const QHostAddress &address, int port, QObject *parent = 0);
    ~ChatServer();

signals:

private slots:
    void onNewConnection();
};

#endif // CHATSERVER_H
