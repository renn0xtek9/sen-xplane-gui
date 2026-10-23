#include <QDebug>

#include <x_plane_aircraft_hmi/aircraftplugin.h>
#include <x_plane_aircraft_hmi/sen_component.h>

AircraftPlugin::AircraftPlugin() {}

AircraftPlugin::~AircraftPlugin() {
  qCritical() << "Aircraft Plugin destroyed";
  if (aircraft_backend_) {
    delete aircraft_backend_;
  }
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

QList<HmiPluginPanel> AircraftPlugin::panels() const {
  return {
      {QStringLiteral("aircraft"), QStringLiteral("Aircraft"), QUrl(QStringLiteral("qrc:/aircraft/AircraftPanel.qml"))},
      {QStringLiteral("aircraftGraph"), QStringLiteral("Altitude Graph"),
       QUrl(QStringLiteral("qrc:/aircraft/AltitudeGraph.qml"))}};
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
