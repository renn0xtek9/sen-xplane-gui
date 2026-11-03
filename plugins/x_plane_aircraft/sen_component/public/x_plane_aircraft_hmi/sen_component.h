#pragma once
#include <sen/kernel/component.h>
#include <sen/kernel/component_api.h>
#include <sen/kernel/kernel.h>

#include <stl/x_plane_aircraft_hmi/config.stl.h>

#include <x_plane_aircraft_hmi/aircraft_frontend.h>

class SEN_EXPORT XplaneAircraftHmiSenComponent : public sen::kernel::Component {
public:
  XplaneAircraftHmiSenComponent() = default;
  ~XplaneAircraftHmiSenComponent() override = default;

  void setAircraftFrontend(AircraftFrontend* aircraftBackend);

  sen::kernel::FuncResult load([[maybe_unused]] sen::kernel::LoadApi&& api) override;

  sen::kernel::PassResult init([[maybe_unused]] sen::kernel::InitApi&& /*api*/) override;

  sen::kernel::FuncResult run([[maybe_unused]] sen::kernel::RunApi& api) override;

  sen::kernel::FuncResult unload([[maybe_unused]] sen::kernel::UnloadApi&& /*api*/) override;

private:
  x_plane_aircraft_hmi::Configuration config_;
  AircraftFrontend* aircraft_backend_{nullptr};

  void updateUIWithNewValues();

  double altitude_{1000.0};
  double speed_{200.0};
  int counter{};
};
