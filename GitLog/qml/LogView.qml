import QtQuick 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.15
import GitTools 1.0

Rectangle {
    color: "antiquewhite"
    border.color: "red"
    border.width: 1
    clip: true

    ListView {
        id: logView
        model: gitlog.logModel
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

        }

    }

}

