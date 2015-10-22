import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import MyChat 1.0

Window {
    visible: true
    minimumWidth: 400
    minimumHeight: 300

    ChatClient {
        id: chatClient
        onStateChanged: {
            if (state===ChatClient.Connected)
            {
                stackView.push(chatForm);
            }
        }
        onNewMessage: {
            chatForm.newMessage(user, text);
        }
        onInfoMessage: {
            chatForm.infoMessage(text);
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: loginForm
    }

    LoginForm {
        id: loginForm
        anchors.fill: parent
    }

    ChatForm {
        id: chatForm
        visible: false
    }
}
