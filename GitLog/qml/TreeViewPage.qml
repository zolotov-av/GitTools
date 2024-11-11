import QtQuick 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.15

Rectangle {
    id: xView
    color: listView.activeFocus ? "lightblue" : "lightgrey"
    clip: true

    // Добавляем это свойство
    StackView.onActivated: {
        console.log("xView StackView.onActivating");
        listView.forceActiveFocus()
    }

    ListView {
        id: listView
        model: gitlog.filesModel
        activeFocusOnTab: true
        anchors.fill: parent

        function activeItem(index) {

            const fileName = listView.model.data(listView.model.index(index, 0), gitlog.filesModel.FileNameRole)

            gitlog.openDiff(index);

            stackView.push("DiffPage.qml", {
                               selectedIndex: index,
                               selectedFileName: fileName // передаём fileName в новую страницу
                           })
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                console.log("listView Keys.onPressed");
                activeItem(listView.currentIndex);
                event.accepted = true;
            }
        }

        delegate: Item {

            implicitHeight: txt.implicitHeight
            implicitWidth: listView.width

            Text {
                id: txt
                text: fileName + " (" + fileStatus + ")"
                color: fileColor
                font.family: root.fontFamily
                font.pixelSize: root.fontSize
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex = index;
                    listView.forceActiveFocus();
                }

                onDoubleClicked: {
                    listView.currentIndex = index;
                    listView.activeItem(index);
                }
            }
        }

        highlight: Item {

            Rectangle {
                x: 0
                y: 0
                height: parent.height
                width: listView.width
                color: "cornflowerblue"
                radius: 5
            }
        }
    }

}
