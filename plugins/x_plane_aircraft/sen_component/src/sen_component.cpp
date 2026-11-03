#include <QDebug>

#include <sen/core/meta/var.h>
#include <sen/kernel/component.h>

#include <stl/sen/kernel/basic_types.stl.h>
#include <stl/x_plane_aircraft_hmi/config.stl.h>

#include <x_plane_aircraft_hmi/sen_component.h>

XplaneAircraftHmiSenComponent::XplaneAircraftHmiSenComponent() {
  qCritical() << "XplaneAircraftHmiSenComponent constructed !";
}

XplaneAircraftHmiSenComponent::~XplaneAircraftHmiSenComponent() {
  qCritical() << "XplaneAircraftHmiSenComponent destroyed !";
}

void XplaneAircraftHmiSenComponent::setAircraftFrontend(AircraftFrontend* aircraft_backend) {
  aircraft_backend_ = aircraft_backend;
}

sen::kernel::FuncResult XplaneAircraftHmiSenComponent::load(sen::kernel::LoadApi&& api) {
  // get our configuration
  config_ = sen::toValue<x_plane_aircraft_hmi::Configuration>(api.getConfig());
  std::cout << "XplaneAircraftHmiSenComponent config:\n" << config_ << "\n";
  return done();
}

sen::kernel::PassResult XplaneAircraftHmiSenComponent::init(sen::kernel::InitApi&& /*api*/) {
  qInfo() << "XplaneAircraftHmiSenComponent init";
  return done();
}

sen::kernel::FuncResult XplaneAircraftHmiSenComponent::run(sen::kernel::RunApi& api) {
  qInfo() << "XplaneAircraftHmiSenComponent run";
  const sen::kernel::BusAddress targetBus{"hmi", "mybus"};
  auto source = api.getSource(targetBus);

  return api.execLoop(sen::Duration::fromHertz(1.0), [&source, this]() {
    std::cout << "XplaneAircraftHmiSenComponent run iteration\n";
    this->updateUIWithNewValues();
    this->counter++;
  });
}

sen::kernel::FuncResult XplaneAircraftHmiSenComponent::unload(sen::kernel::UnloadApi&& /*api*/) {
  qInfo() << "XplaneAircraftHmiSenComponent unload";
  return done();
}

void XplaneAircraftHmiSenComponent::updateUIWithNewValues() {
  const double elapsedMilliseconds = static_cast<double>(this->counter * 1000);
  const double phase = 2.0 * std::numbers::pi * elapsedMilliseconds;
  altitude_ = 1000.0 + 10.0 * std::sin(phase / 10000.0);
  speed_ = 200.0 + 10.0 * std::sin(phase / 5000.0);
  this->aircraft_backend_->setAltitude(altitude_);
  this->aircraft_backend_->setSpeed(speed_);
  this->aircraft_backend_->telemetryChanged();
}

SEN_COMPONENT(XplaneAircraftHmiSenComponent)
