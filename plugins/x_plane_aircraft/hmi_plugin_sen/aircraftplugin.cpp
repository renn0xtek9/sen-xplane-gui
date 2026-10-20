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

  m_sen_component = std::make_shared<XplaneAircraftHmiSenComponent>();
  aircraft_backend_ = new AircraftBackend(parent);
  m_sen_component->setAircraftBackend(aircraft_backend_);
  return aircraft_backend_;
}

std::shared_ptr<sen::kernel::Component> AircraftPlugin::getSenComponent() const {
  if (m_sen_component == nullptr) {
    qCritical() << "Sen Component not yet initialized!";
  }
  qInfo() << "Returning Sen component: " << m_sen_component.get();
  return m_sen_component;
}
