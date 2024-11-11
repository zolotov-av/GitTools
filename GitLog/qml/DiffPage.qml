import QtQuick 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.15

Rectangle {
    id: root
    color: "peachpuff"
    property int selectedIndex: -1
    property string selectedFileName: ""

    function exit() {
        gitlog.closeDiff();
        stackView.pop();
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Backspace || event.key === Qt.Key_Escape) {
            console.log("DiffPage Key_Backspace pressed");
            exit();
            event.accepted = true;
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {

            Text {
                font.family: diffView.fontFamily
                font.pixelSize: diffView.fontSize
                text: root.selectedFileName
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Back")

                onClicked: root.exit()
            }

        }

        DiffArea {
            id: diffView
            model: gitlog.diffModel
            Layout.fillWidth: true
            Layout.fillHeight: true
        }


    }


}
