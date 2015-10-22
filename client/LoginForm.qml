import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

Item {
    anchors.fill: parent
    LinearGradient {
        anchors.fill: parent
        start: Qt.point(0, 0)
        end: Qt.point(0, parent.height)
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightblue" }
            GradientStop { position: 0.5; color: "lightblue" }
            GradientStop { position: 1.0; color: "black" }
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: 7
        Image {
            width: parent.width
            fillMode: Image.PreserveAspectFit
            source: "qrc:/icons/logo.png"
            antialiasing: true
        }
        TextField {
            id: txtHost
            anchors.horizontalCenter: parent
            width: 200
            placeholderText: "Адрес сервера"
        }
        TextField {
            id: txtUserName
            anchors.horizontalCenter: parent
            width: 200
            placeholderText: "Имя пользователя"
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Войти"
            onClicked: {
                chatClient.connectToServer(txtHost.text, 8181, txtUserName.text);
            }
        }
    }
}

