#pragma once
#include <sen/kernel/component.h>
#include <sen/kernel/component_api.h>
#include <sen/kernel/kernel.h>

#include <stl/x_plane_aircraft_hmi/config.stl.h>

#include <x_plane_aircraft_hmi/aircraftbackend.h>

class SEN_EXPORT XplaneAircraftHmiSenComponent : public sen::kernel::Component {
public:
  XplaneAircraftHmiSenComponent();
  virtual ~XplaneAircraftHmiSenComponent();

  void setAircraftBackend(AircraftBackend* aircraft_backend);

  sen::kernel::FuncResult load(sen::kernel::LoadApi&& api) override;

  sen::kernel::PassResult init(sen::kernel::InitApi&& /*api*/) override;

  sen::kernel::FuncResult run(sen::kernel::RunApi& api) override;

  sen::kernel::FuncResult unload(sen::kernel::UnloadApi&& /*api*/) override;

private:
  x_plane_aircraft_hmi::Configuration config_;
  AircraftBackend* aircraft_backend_;

  void updateUIWithNewValues();

  double altitude_{1000.0};
  double speed_{200.0};
  int counter{};

};
