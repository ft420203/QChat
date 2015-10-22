#include "chatserver.h"
#include <QTcpSocket>
#include <iostream>

using namespace std;

ChatServer::ChatServer(QObject *parent) : QObject(parent)
{
    server.listen(QHostAddress::Any, 8181);
    connect(&server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void ChatServer::newConnection()
{
    while (server.hasPendingConnections())
    {
        cout<<"New connection!\n";
        QTcpSocket* socket = server.nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
        m_clients.append(socket);
    }
}

void ChatServer::onReadyRead()
{
    QTcpSocket* socket = (QTcpSocket*)QObject::sender();
    QByteArray data = socket->readAll();
    cout<<data.data();
    m_recievedBytes[socket].append(data);
    if (m_recievedBytes[socket].length()>1024*1024)
    {
        rejectConnection(socket);
        return;
    }
    int pos = m_recievedBytes[socket].indexOf("\r\n");
    while (pos>-1)
    {
        QByteArray line = m_recievedBytes[socket].left(pos);
        m_recievedBytes[socket] = m_recievedBytes[socket].mid(pos+2);
        handleLine(line, socket);
        pos = m_recievedBytes[socket].indexOf("\r\n");
    }
}

void ChatServer::onDisconnected()
{
    cout<<"Disconnected!\n";
    QTcpSocket* socket = (QTcpSocket*)QObject::sender();
    if (m_users.contains(socket))
    {
        sendToAll("info Пользователь "+m_users[socket]+" покинул чат\r\n");
    }
    clearConnectionData(socket);
}

void ChatServer::clearConnectionData(QTcpSocket *socket)
{
    m_recievedBytes.remove(socket);
    m_clients.removeOne(socket);
    m_users.remove(socket);
}

void ChatServer::rejectConnection(QTcpSocket *socket)
{
    clearConnectionData(socket);
    socket->close();
}

QString ChatServer::eatWord(QByteArray &data)
{
    int pos = data.indexOf(" ");
    if (pos>-1)
    {
        QByteArray word = data.left(pos);
        data = data.mid(pos+1);
        return QString::fromUtf8(word);
    } else
    {
        return QString::fromUtf8(data);
    }
}

QString ChatServer::eatAll(QByteArray &data)
{
    return QString::fromUtf8(data);
    data.clear();
}

bool ChatServer::correctUserName(QString userName)
{
    foreach (QChar c, userName) {
        if (!c.isDigit() && !c.isLetter())
        {
            return false;
        }
    }
    return true;
}

void ChatServer::sendToAll(QString str)
{
    foreach (QTcpSocket* socket, m_users.keys()) {
        socket->write(str.toUtf8());
    }
}

void ChatServer::handleLine(QByteArray line, QTcpSocket socket)
{
    QString cmd = eatWord(line);
    if (cmd=="connection")
    {
        QString userName = eatAll(line);
        if (correctUserName(userName))
        {
            socket->write("connection OK\r\n");
            m_users[socket] = userName;
            sendToAll("info Пользователь "+userName+" присоединился к чату\r\n");
        } else
        {
            socket->write("connection Error\r\n");
            rejectConnection(socket);
        }
        return;
    }
    if (cmd=="message")
    {
        sendToAll("message "+QString::fromUtf8(line)+"\r\n");
    }
}
