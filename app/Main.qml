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

    ListModel {
        id: panelModel

        ListElement {
            title: "Aircraft"
            shown: true
        }
        ListElement {
            title: "Map"
            shown: true
        }
    }

    ListModel {
        id: pluginModel

        ListElement {
            name: "X-Plane aircraft"
            description: "Aircraft telemetry will appear here when the bridge is connected."
            active: true
        }
        ListElement {
            name: "OpenStreetMap"
            description: "The aircraft's position on the map will appear here."
            active: true
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Exit"
                onTriggered: Qt.quit()
            }
        }

        Menu {
            title: "View"
            Repeater {
                model: panelModel

                delegate: MenuItem {
                    required property string title
                    required property bool shown
                    required property int index

                    text: title
                    checkable: true
                    checked: shown
                    onTriggered: panelModel.setProperty(index, "shown", checked)
                }
            }
        }

        Menu {
            title: "Plugin"
            Repeater {
                model: pluginModel

                delegate: MenuItem {
                    required property string name
                    required property bool active
                    required property int index

                    text: name
                    checkable: true
                    checked: active
                    onTriggered: pluginModel.setProperty(index, "active", checked)
                }
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Repeater {
            model: panelModel

            delegate: Rectangle {
                required property string title
                required property bool shown
                required property int index

                visible: shown && pluginModel.get(index).active
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 300
                radius: 6
                color: index === 0 ? "#182829" : "#1c2924"
                border.color: index === 0 ? "#345454" : "#405c49"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 28
                    spacing: 12

                    Label {
                        text: title
                        color: "#e7f0e8"
                        font.pixelSize: 24
                        font.weight: Font.DemiBold
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: "#354747"
                    }

                    Label {
                        Layout.fillWidth: true
                        text: pluginModel.get(index).description
                        color: "#9eb3aa"
                        font.pixelSize: 16
                        wrapMode: Text.WordWrap
                    }

                    Item {
                        Layout.fillHeight: true
                    }

                    Label {
                        text: index === 0 ? "ALTITUDE     --     SPEED     --" : "POSITION     --"
                        color: "#75d3ad"
                        font.family: "monospace"
                        font.pixelSize: 16
                    }
                }
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
