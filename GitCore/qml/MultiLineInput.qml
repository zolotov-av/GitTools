import QtQuick 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.15
import GitTools 1.0

FocusScope {
    id: root
    implicitWidth: layout.implicitWidth
    implicitHeight: layout.implicitHeight

    property alias label: label.text
    property alias comment: comment.text
    property alias errorMessage: error.text
    property alias placeholderText: textArea.placeholderText
    property alias text: textArea.text

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 4

        Label {
            id: label
            text: "Field label"
            font.bold: true
            font.pixelSize: GitTheme.smallerFont
        }
        Label {
            id: comment
            visible: text.length > 0
            font.pointSize: GitTheme.smallFont
            color: "#666666"
        }
        ScrollView {
            id: scrollView
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                id: textArea
                placeholderText: qsTr("Author name")
                padding: 4

                background: Rectangle {
                    border.color: "lightgrey"
                    border.width: 1
                    color: "transparent"
                }
            }
        }
        Label {
            id: error
            visible: text.length > 0
            color: "#d32f2f"
            font.pointSize: GitTheme.smallerFont
            Layout.alignment: Qt.AlignRight
        }
    }

}
