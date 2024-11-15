import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import GitTools 1.0

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
        spacing: 12

        LineInput {
            label: qsTr("Author")
            placeholderText: qsTr("Enter name")
            text: form.authorName
            Layout.fillWidth: true
        }

        LineInput {
            label: qsTr("E-mail")
            placeholderText: qsTr("Enter e-mail")
            text: form.authorEmail
            Layout.fillWidth: true
        }

        MultiLineInput {
            label: qsTr("Message")
            placeholderText: qsTr("Enter description")
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 4

            Label {
                text: qsTr("Files:")
                font.bold: true
                font.pixelSize: GitTheme.smallerFont
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
