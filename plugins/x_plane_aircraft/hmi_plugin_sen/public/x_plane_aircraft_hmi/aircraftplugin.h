#pragma once

#include <hmi/plugin_sen.h>

#include <x_plane_aircraft_hmi/aircraft_frontend.h>
#include <x_plane_aircraft_hmi/sen_component.h>

#include <QObject>

class AircraftPlugin final : public QObject, public HmiPluginSen {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID HMI_PLUGIN_SEN_IID)
  Q_INTERFACES(HmiPluginSen)

public:
  AircraftPlugin();
  virtual ~AircraftPlugin();
  [[nodiscard]] QString id() const override;
  [[nodiscard]] QString name() const override;
  [[nodiscard]] QString panelTitle() const override;
  [[nodiscard]] QString description() const override;
  [[nodiscard]] QUrl panelSource() const override;
  [[nodiscard]] QObject* createBackend(QObject* parent) override;
  [[nodiscard]] std::shared_ptr<sen::kernel::Component> getSenComponent() const override;

private:
  AircraftFrontend* aircraft_backend_{};
  std::shared_ptr<XplaneAircraftHmiSenComponent> m_sen_component;
};
