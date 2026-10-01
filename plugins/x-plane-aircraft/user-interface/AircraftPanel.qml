import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    property var backend

    anchors.fill: parent
    radius: 6
    color: "#182829"
    border.color: "#345454"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 12

        Label {
            text: "Aircraft"
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
            text: "Simulated aircraft telemetry"
            color: "#9eb3aa"
            font.pixelSize: 16
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 24

            ColumnLayout {
                spacing: 4

                Label {
                    text: "ALTITUDE"
                    color: "#9eb3aa"
                    font.pixelSize: 12
                }

                Label {
                    text: backend ? backend.altitude.toFixed(1) + " m" : "--"
                    color: "#75d3ad"
                    font.family: "monospace"
                    font.pixelSize: 24
                }
            }

            ColumnLayout {
                spacing: 4

                Label {
                    text: "SPEED"
                    color: "#9eb3aa"
                    font.pixelSize: 12
                }

                Label {
                    text: backend ? backend.speed.toFixed(1) + " km/h" : "--"
                    color: "#75d3ad"
                    font.family: "monospace"
                    font.pixelSize: 24
                }
            }
        }
    }
}
