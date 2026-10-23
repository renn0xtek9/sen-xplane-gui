import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 960
    height: 600
    title: "X-Plane Aircraft UI Evaluation"
    color: "#101b1c"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        Label {
            text: "Aircraft panel evaluation"
            color: "#e7f0e8"
            font.pixelSize: 24
            font.weight: Font.DemiBold
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 24

            ColumnLayout {
                Layout.preferredWidth: 280
                Layout.fillHeight: true
                spacing: 12

                Label {
                    text: "Altitude: " + Math.round(altitudeSlider.value) + " m"
                    color: "#e7f0e8"
                }

                Slider {
                    id: altitudeSlider
                    Layout.fillWidth: true
                    from: 0
                    to: 12000
                    value: aircraftFrontend.altitude
                    onMoved: aircraftFrontend.altitude = value
                }

                Label {
                    text: "Speed: " + Math.round(speedSlider.value) + " km/h"
                    color: "#e7f0e8"
                }

                Slider {
                    id: speedSlider
                    Layout.fillWidth: true
                    from: 0
                    to: 1000
                    value: aircraftFrontend.speed
                    onMoved: aircraftFrontend.speed = value
                }

                Item {
                    Layout.fillHeight: true
                }
            }

            Loader {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 400
                Layout.preferredWidth: 600
                source: "qrc:/aircraft/AltitudeGraph.qml"
                onLoaded: item.backend = aircraftFrontend
            }
        }
    }
}
