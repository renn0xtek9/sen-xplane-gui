#include <QDebug>
#include <QMetaObject>
#include <Qt>

#include <cmath>
#include <numbers>

#include <sen/core/base/duration.h>
#include <sen/core/io/util.h>
#include <sen/core/meta/var.h>
#include <sen/kernel/component.h>
#include <sen/kernel/component_api.h>

#include <stl/sen/kernel/basic_types.stl.h>
#include <stl/x_plane_aircraft_hmi/config.stl.h>

#include <x_plane_aircraft_hmi/aircraft_frontend.h>
#include <x_plane_aircraft_hmi/sen_component.h>

void XplaneAircraftHmiSenComponent::addAircraftFrontend(AircraftFrontend* aircraftBackend) {
  aircraft_backends_.push_back(aircraftBackend);
}

// NOLINTNEXTLINE(cppcoreguidelines-rvalue-reference-param-not-moved): Required non-movable Sen lifecycle API.
sen::kernel::FuncResult XplaneAircraftHmiSenComponent::load([[maybe_unused]] sen::kernel::LoadApi&& api) {
  config_ = sen::toValue<x_plane_aircraft_hmi::Configuration>(api.getConfig());
  return done();
}

sen::kernel::PassResult XplaneAircraftHmiSenComponent::init([[maybe_unused]] sen::kernel::InitApi&& /*api*/) {
  return done();
}

sen::kernel::FuncResult XplaneAircraftHmiSenComponent::run([[maybe_unused]] sen::kernel::RunApi& api) {
  const sen::kernel::BusAddress targetBus{.sessionName = "hmi", .busName = "mybus"};
  auto source = api.getSource(targetBus);

  return api.execLoop(sen::Duration::fromHertz(1.0), [this]() {
    this->updateUIWithNewValues();
    this->counter++;
  });
}

sen::kernel::FuncResult XplaneAircraftHmiSenComponent::unload([[maybe_unused]] sen::kernel::UnloadApi&& /*api*/) {
  return done();
}

void XplaneAircraftHmiSenComponent::updateUIWithNewValues() {
  // TODO this calculation belong to the aircraft_backend
  const auto elapsedMilliseconds = static_cast<double>(this->counter * 1000);
  const double phase = 2.0 * std::numbers::pi * elapsedMilliseconds;
  altitude_ = 1000.0 + (10.0 * std::sin(phase / 10000.0));
  speed_ = 200.0 + (10.0 * std::sin(phase / 5000.0));

  for (auto* backend : aircraft_backends_) {
    QMetaObject::invokeMethod(
        backend,
        [frontend = backend, this]() {
          frontend->setAltitude(altitude_);
          frontend->setSpeed(speed_);
        },
        Qt::QueuedConnection);
  }
}

SEN_COMPONENT(XplaneAircraftHmiSenComponent)
