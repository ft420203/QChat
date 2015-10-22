#include "chatclient.h"
#include <QDebug>

ChatClient::ChatClient(QObject *parent) : QObject(parent)
{
    connect(&m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(&m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError()));
}

QString ChatClient::userName()
{
    return m_userName;
}

ChatClient::State ChatClient::state()
{
    return m_state;
}

void ChatClient::sendMessage(QString text)
{
    QByteArray data;
    data.append("message ");
    data.append(m_userName.toUtf8());
    data.append(" ");
    data.append(text.toUtf8().toPercentEncoding()+"\r\n");
    m_socket.write(data);
}

void ChatClient::connectToServer(QString host, qint16 port, QString userName)
{
    m_socket.connectToHost(host, port);
    m_userName = userName;
}

void ChatClient::onReadyRead()
{
    m_recievedBytes.append(m_socket.readAll());
    int pos = m_recievedBytes.indexOf("\r\n");
    while (pos>-1)
    {
        QByteArray line = m_recievedBytes.left(pos);
        m_recievedBytes = m_recievedBytes.mid(pos+2);
        parseLine(line);
        pos = m_recievedBytes.indexOf("\r\n");
    }
}

void ChatClient::onConnected()
{
    emit infoMessage(tr("Соединение с сервером установлено"));
    m_socket.write(QString("connection "+m_userName+"\r\n").toUtf8());
}

void ChatClient::onDisconnected()
{
    emit infoMessage(tr("Соединение с сервером разорвано"));
    m_state = Disconnected;
    emit stateChanged();
}

void ChatClient::onError()
{
    infoMessage(tr("Сетевая ошибка: ")+m_socket.errorString());
}

QString ChatClient::eatWord(QByteArray &data)
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

QString ChatClient::eatAll(QByteArray &data)
{
    return QString::fromUtf8(data);
    data.clear();
}

void ChatClient::parseLine(QByteArray line)
{
    QString cmd = eatWord(line);
    if (cmd=="message")
    {
        QString user = eatWord(line);
        QString message = QString::fromUtf8(QByteArray::fromPercentEncoding(line));
        line.clear();
        emit newMessage(user, message);
        return;
    }
    if (cmd=="connection")
    {
        QString arg0 = eatWord(line);
        if (arg0=="OK")
        {
            m_state = Connected;
            emit stateChanged();
            line.clear();
            return;
        } else
        {
            m_state = Disconnected;
            emit stateChanged();
            QString error = eatAll(line);
            emit connectionError(error);
            return;
        }
    }
    if (cmd=="info")
    {
        QString message = eatAll(line);
        emit infoMessage(message);
        return;
    }
}
