import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import Qt.labs.settings 1.0
import Qt.labs.platform 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 850
    height: 650
    title: qsTr("Git Log")

    function showWindow() {
        window.show()
        window.raise()
        window.requestActivate()
    }

    // Восстановление размера окна
    Component.onCompleted: {
        if (gitlog.configValue("window/maximized", "no") === "yes") {
            window.showMaximized()
        } else {
            const w = parseInt(gitlog.configValue("window/width", "850"))
            const h = parseInt(gitlog.configValue("window/height", "650"))
            if (w > 0 && h > 0) {
                window.width = w
                window.height = h
            }
        }
    }

    onWidthChanged: {
        if ( !(window.visibility === Window.Maximized) ) {
            gitlog.setConfigValue("window/width", width.toString())
        }
    }

    onHeightChanged: {
        if ( !(window.visibility === Window.Maximized) ) {
            gitlog.setConfigValue("window/height", height.toString())
        }
    }

    onVisibilityChanged: {
        gitlog.setConfigValue("window/maximized",
                              window.visibility === Window.Maximized ? "yes" : "no")
    }

    // Обработка закрытия окна
    onClosing: function(close) {
        if (gitlog.minimizeToTray) {
            close.accepted = false
            window.hide()
        }
    }

    // Подключаем обработчик сигнала
    Connections {
        target: gitlog
        function onShowWindowRequested() {
            window.showWindow()
        }
    }
    SystemTrayIcon {
        id: systemTray
        visible: true
        iconSource: "qrc:/icons/folder-blue-git-icon.png"
        tooltip: window.title

        onActivated: {
            if (reason === SystemTrayIcon.Trigger) {
                if (window.visible) {
                    window.hide()
                } else {
                    window.showWindow()
                }
            }
        }

        menu: Menu {
            MenuItem {
                text: qsTr("Exit")
                onTriggered: gitlog.exit()
            }
        }
    }

    GitLog {
        anchors.fill: parent
    }
}
