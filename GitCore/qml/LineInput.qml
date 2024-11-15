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
    property alias placeholderText: textField.placeholderText
    property alias text: textField.text

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
        TextField {
            id: textField
            placeholderText: "Enter field value..."
            Layout.fillWidth: true
            padding: 4
            font.pixelSize: GitTheme.fontSize

            background: Rectangle {
                color: "transparent"
                border.color: "lightgrey"
                border.width: 1
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
