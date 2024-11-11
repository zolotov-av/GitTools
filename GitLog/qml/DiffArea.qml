import QtQuick 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.15

Rectangle {
    id: root
    color: "white"
    clip: true
    property alias model: lv.model
    property int fontSize: 18
    property string fontFamily: "Monospace"
    property int lineNumberWidth: 50
    property alias readOnly: editor.readOnly

    Rectangle {
        color: "burlywood"
        width: root.lineNumberWidth
        height: parent.height
    }

    ListView {
        id: lv
        anchors.fill: parent
        interactive: false

        // Синхронизируем с Flickable
        contentY: scrollView.contentItem.contentY

        delegate: Rectangle {
            implicitHeight: txt.implicitHeight
            width: lv.width

            color: lineColor
            Row {
                spacing: 5
                Rectangle {
                    implicitWidth: root.lineNumberWidth
                    implicitHeight: ln.implicitHeight
                    color: "burlywood"
                    Text {
                        id: ln
                        text: index + 1
                        font.family: root.fontFamily
                        font.pixelSize: root.fontSize
                        anchors.fill: parent
                        anchors.margins: 2
                        horizontalAlignment: Qt.AlignRight
                    }
                }

                Text {
                    id: txt
                    visible: false
                    text: lineText
                    font.family: root.fontFamily
                    font.pixelSize: root.fontSize
                }
            }

        }
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        clip: true

        TextEdit {
            id: editor
            font.family: root.fontFamily
            font.pixelSize: root.fontSize
            wrapMode: TextEdit.NoWrap
            text: lv.model.text
            selectByMouse: true // Разрешаем выделение мышью
            mouseSelectionMode: TextEdit.SelectCharacters

            leftPadding: root.lineNumberWidth + 4
            rightPadding: 4
            topPadding: 0
            bottomPadding: 0
        }
    }

}
