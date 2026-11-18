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
  delete aircraft_frontend_;
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

QList<hmi::PluginView> AircraftPlugin::views() const {
  return {{.id = QStringLiteral("aircraft"),
           .title = QStringLiteral("Aircraft"),
           .source = QUrl(QStringLiteral("qrc:/aircraft/AircraftPanel.qml"))},
          {.id = QStringLiteral("aircraftGraph"),
           .title = QStringLiteral("Altitude Graph"),
           .source = QUrl(QStringLiteral("qrc:/aircraft/AltitudeGraph.qml"))}};
}

QObject* AircraftPlugin::instantiateView(const QString viewId, QObject* parent) {
  createSenComponentIfUnitialized();
  aircraft_frontend_ = new AircraftFrontend(parent);
  m_sen_component->addAircraftFrontend(aircraft_frontend_);
  return aircraft_frontend_;
}

std::shared_ptr<sen::kernel::Component> AircraftPlugin::getSenComponent() {
  createSenComponentIfUnitialized();
  return m_sen_component;
}

void AircraftPlugin::createSenComponentIfUnitialized() {
  if (m_sen_component == nullptr) {
    m_sen_component = std::make_shared<XplaneAircraftHmiSenComponent>();
  } else {
    qInfo() << "Sen component already initialized for plugin: " << name();
  }
}
