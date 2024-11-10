import QtQuick 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.15

Item {
    id: root
    property alias headerVisible: header.visible
    property int fontSize: 18
    property string fontFamily: "Monospace"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: header
            color: "lightgrey"
            Layout.fillWidth: true
            implicitHeight: grid.implicitHeight + grid.anchors.margins * 2
            visible: true

            GridLayout {
                id: grid
                columns: 2
                columnSpacing: 4
                rowSpacing: columnSpacing
                anchors.fill: parent
                anchors.margins: 8

                TextInput {
                    text: diff.oldFileName
                    Layout.fillWidth: true
                    font.pixelSize: root.fontSize
                    font.family: root.fontFamily
                }

                TextInput {
                    text: diff.newFileName
                    Layout.fillWidth: true
                    font.pixelSize: root.fontSize
                    font.family: root.fontFamily
                    horizontalAlignment: Qt.AlignRight
                }
            }
        }

        GitDiffArea {
            id: rightView
            fontFamily: root.fontFamily
            fontSize: root.fontSize
            model: lineView.lineModel
            readOnly: true
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

    }

}
