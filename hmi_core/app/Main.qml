import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

ApplicationWindow {
    id: window

    width: 1280
    height: 800
    visible: true
    visibility: Window.FullScreen
    title: "Sen X-Plane HMI"
    color: "#101b1c"

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Connect to Sen"
                onTriggered: senBridge.connectToSen()
            }
            MenuSeparator {}
            MenuItem {
                text: "Exit"
                onTriggered: Qt.quit()
            }
        }

        Menu {
            title: "View"
            Repeater {
                model: pluginManager.panels

                delegate: MenuItem {
                    required property string pluginId
                    required property string panelId
                    required property string panelTitle
                    required property bool panelVisible
                    required property string pluginName

                    text: pluginName + " — " + panelTitle
                    checkable: true
                    checked: panelVisible
                    onTriggered: pluginManager.setPanelVisible(pluginId, panelId, checked)
                }
            }
        }

        Menu {
            title: "Plugin"
            Repeater {
                model: pluginManager.plugins

                delegate: MenuItem {
                    required property string pluginId
                    required property string pluginName
                    required property bool pluginActive

                    text: pluginName
                    checkable: true
                    checked: pluginActive
                    onTriggered: pluginManager.setPluginActive(pluginId, checked)
                }
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Repeater {
            model: pluginManager.panels

            delegate: Loader {
                required property string pluginId
                required property string panelId
                required property bool pluginActive
                required property bool panelVisible
                required property var backend
                required property url panelSource

                active: pluginActive && panelVisible
                visible: active
                source: panelSource
                Layout.fillWidth: active
                Layout.fillHeight: true
                Layout.minimumWidth: active ? 300 : 0
                Layout.preferredWidth: active ? 1 : 0
                onLoaded: item.backend = backend
            }
        }
    }

    footer: ToolBar {
        contentItem: RowLayout {
            spacing: 10

            Rectangle {
                width: 8
                height: 8
                radius: 4
                color: hmiState.connected ? "#67d5a5" : "#e2ad61"
            }

            Label {
                text: hmiState.connected ? "Connected" : "Not connected"
                color: "#e7f0e8"
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }
}
