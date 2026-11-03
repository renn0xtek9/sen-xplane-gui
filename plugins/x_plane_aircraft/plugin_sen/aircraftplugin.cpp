#include <QDebug>
#include <QList>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QtLogging>

#include <memory>

#include <sen/kernel/component.h>

#include <hmi/plugin_interface.h>

#include <x_plane_aircraft_hmi/aircraft_frontend.h>
#include <x_plane_aircraft_hmi/aircraftplugin.h>
#include <x_plane_aircraft_hmi/sen_component.h>

AircraftPlugin::~AircraftPlugin() {
  qCritical() << "Aircraft Plugin destroyed";
  delete aircraft_backend_;
}

QString AircraftPlugin::id() const {
  return "x_plane_aircraft";
}

QString AircraftPlugin::name() const {
  return "X-Plane aircraft";
}

QString AircraftPlugin::description() const {
  return "Simulated aircraft telemetry.";
}

QList<hmi::PluginPanel> AircraftPlugin::panels() const {
  return {{.id = QStringLiteral("aircraft"),
           .title = QStringLiteral("Aircraft"),
           .source = QUrl(QStringLiteral("qrc:/aircraft/AircraftPanel.qml"))},
          {.id = QStringLiteral("aircraftGraph"),
           .title = QStringLiteral("Altitude Graph"),
           .source = QUrl(QStringLiteral("qrc:/aircraft/AltitudeGraph.qml"))}};
}

QObject* AircraftPlugin::createBackend(QObject* parent) {

  m_sen_component = std::make_shared<XplaneAircraftHmiSenComponent>();
  aircraft_backend_ = new AircraftFrontend(parent);
  m_sen_component->setAircraftFrontend(aircraft_backend_);
  return aircraft_backend_;
}

std::shared_ptr<sen::kernel::Component> AircraftPlugin::getSenComponent() const {
  if (m_sen_component == nullptr) {
    qCritical() << "Sen Component not yet initialized!";
  }
  qInfo() << "Returning Sen component: " << m_sen_component.get();
  return m_sen_component;
}
