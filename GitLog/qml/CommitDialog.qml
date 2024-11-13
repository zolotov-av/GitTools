import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

ApplicationWindow {
    id: window
    title: qsTr("Test Dialog")
    width: 500
    height: 600
    modality: Qt.ApplicationModal
    flags: Qt.Dialog
    //visible: true

    ColumnLayout {

        anchors.fill: parent
        anchors.margins: 10

        GridLayout {
            columns: 2
            Layout.fillWidth: true

            Label {
                text: qsTr("Author:")
                Layout.alignment: Qt.AlignRight
            }

            TextField {
                text: form.authorName
                placeholderText: qsTr("Enter name")
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("E-mail:")
                Layout.alignment: Qt.AlignRight
            }

            TextField {
                text: form.authorEmail
                placeholderText: qsTr("Enter e-mail")
                Layout.fillWidth: true
            }
        }

        Label {
            text: qsTr("Message:")
        }

        TextArea {
            placeholderText: qsTr("Enter description")
            Layout.fillWidth: true
        }

        Label {
            text: qsTr("Files:")
        }

        Frame {
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {

                id: lv
                anchors.fill: parent

                model: filesModel

                delegate: Item {

                    implicitHeight: txt.implicitHeight
                    width: lv.width

                    Item {
                        height: txt.implicitHeight
                        width: parent.width
                        Text {
                            id: txt
                            text: fileName
                            color: fileColor
                        }
                    }
                }
            }
        }

        RowLayout {
            id: buttons

            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Commit")
                onClicked: form.accept()
            }

            Button {
                text: qsTr("Cancel")
                onClicked: form.reject()
            }
        }
    }

}
