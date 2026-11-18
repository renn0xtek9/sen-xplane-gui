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
                model: pluginManager.availableViews

                delegate: MenuItem {
                    required property string pluginId
                    required property string viewId
                    required property string viewTitle
                    required property string pluginName

                    text: pluginName + " — " + viewTitle
                    checkable: false
                    onTriggered: {
                        console.log("Instantiate view:" + pluginId + " " + viewId);
                        pluginManager.instantiateView(pluginId, viewId);
                    }
                }
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Repeater {
            model: pluginManager.instantiatedViews

            delegate: Loader {
                required property var viewControl
                required property url viewSource

                active: true
                visible: true
                source: viewSource
                Layout.fillWidth: active
                Layout.fillHeight: true
                Layout.minimumWidth: active ? 300 : 0
                Layout.preferredWidth: active ? 1 : 0
                onLoaded: item.control = viewControl
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
