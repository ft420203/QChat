#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QHash>
#include <QByteArray>

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = 0);

signals:


public slots:

private slots:
    void newConnection();
    void onReadyRead();
    void onDisconnected();
    void clearConnectionData(QTcpSocket* socket);
    void rejectConnection(QTcpSocket* socket);

private:
    QTcpServer server;
    QHash<QTcpSocket*, QByteArray > m_recievedBytes;
    QList<QTcpSocket* > m_clients;
    QHash<QTcpSocket*, QString> m_users;
    QString eatWord(QByteArray &data);
    QString eatAll(QByteArray &data);
    bool correctUserName(QString userName);
    void sendToAll(QString str);
    void handleLine(QByteArray line, QTcpSocket socket);
};

#endif // CHATSERVER_H
