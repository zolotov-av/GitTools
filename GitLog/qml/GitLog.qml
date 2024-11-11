import QtQuick 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.15

Item {
    id: root
    property int fontSize: 18
    property string fontFamily: "Monospace"

    function debugMessage(msg) {
        console.log("QML debugMessage: " + msg.toString())
        stackView.forceActiveFocus();
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            color: commitMessage.activeFocus ? "lightblue" : "lightgrey"
            implicitHeight: commitMessage.implicitHeight + 10
            Layout.fillWidth: true

            TextEdit {
                id: commitMessage
                activeFocusOnTab: true
                anchors.fill: parent
                anchors.margins: 5
                text: gitlog.commitMessage
                readOnly: true
                selectByKeyboard: true
                selectByMouse: true
            }
        }

        StackView {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: "TreeViewPage.qml"
        }

    }

}
