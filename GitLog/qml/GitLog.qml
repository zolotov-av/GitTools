import QtQuick 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.15
import Qt.labs.platform 1.0
import GitTools 1.0

Item {
    id: root
    property int fontSize: 18
    property string fontFamily: "Monospace"

    function debugMessage(msg) {
        console.log("QML debugMessage: " + msg.toString())
        stackView.forceActiveFocus();
    }

    FolderDialog {
        id: folderDialog
        title: qsTr("Choose repository")

        onAccepted: {
            console.log("QML folderDialog accepted: " + folder)
            gitlog.openRepository(folder)
        }
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            id: buttons

            ToolButton {
                text: qsTr("Open")
                onClicked: folderDialog.open()
            }

            ToolButton {
                text: qsTr("Refresh")

                onClicked: gitlog.update()
            }

            ToolButton {
                id: showAllBranches
                text: qsTr("All branches")
                checkable: true
                checked: false

                Binding {
                    target: gitlog
                    property: "showAllBranches"
                    value: showAllBranches.checked
                }
            }

            ToolButton {
                id: showTags
                text: qsTr("Display tags")
                checkable: true
                checked: false

                Binding {
                    target: gitlog
                    property: "showTags"
                    value: showTags.checked
                }
            }

            ToolButton {
                text: qsTr("Commit")

                onClicked: gitlog.openCommitDialog()
            }
        }

        LogView {
            id: logView
            implicitHeight: 300
            Layout.fillWidth: true
        }

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
