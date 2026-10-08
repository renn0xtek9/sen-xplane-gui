#include "aircraftplugin.h"

#include "aircraftbackend.h"

QString AircraftPlugin::id() const {
  return "x-plane-aircraft";
}

QString AircraftPlugin::name() const {
  return "X-Plane aircraft";
}

QString AircraftPlugin::panelTitle() const {
  return "Aircraft";
}

QString AircraftPlugin::description() const {
  return "Simulated aircraft telemetry.";
}

QUrl AircraftPlugin::panelSource() const {
  return QUrl(QStringLiteral("qrc:/aircraft/AircraftPanel.qml"));
}

QObject* AircraftPlugin::createBackend(QObject* parent) {
  return new AircraftBackend(parent);
}
