import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtMultimedia 5.0
import QtQuick.Controls.Styles 1.4

Item {
    visible: true
    width: 400
    height: 300

    SoundEffect {
        id: sound
        source: "sounds/icq.wav"
    }

    function newMessage(user, text)
    {
        history.append("<b>"+user+": </b>"+text);
        sound.play();
    }

    function infoMessage(text)
    {
        history.append("<font color=\"Green\"><b>Info: </b>"+text+"</font>");
    }

    function sendMessage()
    {
        if (message.text.length>0)
        {
            chatClient.sendMessage(message.text);
            message.text = "";
        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Vertical
        TextArea {
            id: history
            height: parent.height*0.7
            readOnly: true
            textFormat: TextEdit.RichText
            wrapMode: TextEdit.Wrap
            style: TextAreaStyle {
                    textColor: "#333"
                    backgroundColor: "lightblue"
                }
        }
        Row {
            Layout.minimumHeight: 100
            TextArea {
                id: message
                width: parent.width-sendButton.width
                height: parent.height
                wrapMode: TextEdit.Wrap
                focus: true
                Keys.onPressed: {
                    if (event.key == Qt.Key_Control & Qt.Key_Enter)
                    {
                        sendMessage();
                    }
                }
            }
            Button {
                id: sendButton
                width: 70
                height: parent.height
                iconSource: "icons/send.png"
                onClicked: {
                    sendMessage();
                }
            }
        }
    }
}

