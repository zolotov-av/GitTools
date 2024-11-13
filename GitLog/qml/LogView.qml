import QtQuick 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.15
import GitTools 1.0

Rectangle {
    color: logView.activeFocus ? "lightblue" : "lightgrey"
    clip: true

    property alias currentIndex: logView.currentIndex

    ListView {
        id: logView
        model: gitlog.logModel
        activeFocusOnTab: true
        anchors.fill: parent

        delegate: Item {

            implicitHeight: txt.implicitHeight
            implicitWidth: logView.width

            RowLayout {
                anchors.fill: parent

                GraphItem {
                    implicitWidth: 100
                    implicitHeight: txt.implicitHeight

                    model: logView.model
                    time: commitTime
                    itemIndex: index
                }

                Text {
                    id: txt
                    text: commitMessage
                    font.family: root.fontFamily
                    font.pixelSize: root.fontSize
                    Layout.fillWidth: true
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    logView.currentIndex = index;
                    gitlog.showCommit(index);
                    logView.forceActiveFocus();
                }

                onDoubleClicked: {
                    logView.currentIndex = index;
                    logView.activeItem(index);
                }
            }
        }

        highlight: Item {

            Rectangle {
                x: 0
                y: 0
                height: parent.height
                width: logView.width
                color: "cornflowerblue"
                radius: 5
            }
        }

    }

}

