#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>

class ChatClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString userName READ userName)
public:
    enum State
    {
        Connected,
        Disconnected
    };
    Q_ENUMS(State)
    explicit ChatClient(QObject *parent = 0);
    QString userName();
    State state();

signals:
    void newMessage(QString user, QString text);
    void stateChanged();
    void connectionError(QString errorString);
    void infoMessage(QString text);

public slots:
    void sendMessage(QString text);
    void connectToServer(QString host, qint16 port, QString userName);

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError();

private:
    QString m_userName;
    QTcpSocket m_socket;
    QByteArray m_recievedBytes;
    State m_state;
    QString eatWord(QByteArray &data);
    QString eatAll(QByteArray &data);
    void parseLine(QByteArray line);
};

#endif // CHATCLIENT_H
