#include <QDebug>
#include <QMetaObject>
#include <Qt>
#include <QtLogging>

#include <string>

#include <sen/core/base/duration.h>
#include <sen/core/io/util.h>
#include <sen/core/obj/detail/work_queue.h>
#include <sen/core/obj/object_list.h>
#include <sen/kernel/component.h>
#include <sen/kernel/component_api.h>

#include <stl/x_plane_aircraft_hmi/config.stl.h>
#include <stl/x_plane_node/x_plane_node.stl.h>

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
// NOLINTNEXTLINE(cppcoreguidelines-rvalue-reference-param-not-moved): Required non-movable Sen lifecycle API.
sen::kernel::PassResult XplaneAircraftHmiSenComponent::init(sen::kernel::InitApi&& api) {
  api.getTypes().add(x_plane_node::XPlaneNodeInterface::meta());
  work_queue_ = api.getWorkQueue();
  x_plane_subscription_ = api.selectAllFrom<x_plane_node::XPlaneNodeInterface>(
      std::string{"x_plane.aircraft"},
      [this](const sen::ObjectList<x_plane_node::XPlaneNodeInterface>::Iterators& addedObjects) {
        for (auto* object : addedObjects) {
          x_plane_nodes_.push_back(object);
          qInfo() << "Discovered X-Plane node:" << object->asObject().getName();
          const auto updatePose = [this, object]() {
            const auto& pose = object->getPose();
            altitude_ = static_cast<double>(pose.elevationAboveMsl);
            speed_ = static_cast<double>(pose.groundSpeed);
            updateUIWithNewValues();
          };
          updatePose();
          pose_callbacks_.push_back(object->onPoseChanged(sen::PropertyCallback{
              work_queue_,
              sen::PropertyCallback::CallbackFunc{sen::PropertyCallback::Function{[updatePose]() { updatePose(); }}}}));
        }
      },
      [this]([[maybe_unused]] const sen::ObjectList<x_plane_node::XPlaneNodeInterface>::Iterators& removedObjects) {
        pose_callbacks_.clear();
        x_plane_nodes_.clear();
      });
  return done();
}

void XplaneAircraftHmiSenComponent::increaseAltitude() {
  if (work_queue_ == nullptr) {
    qWarning() << "Cannot increase altitude before the Sen component is initialized";
    return;
  }

  work_queue_->push(
      [this]() {
        for (auto* object : x_plane_nodes_) {
          object->increaseAltitute({});
        }
      },
      false);
}

sen::kernel::FuncResult XplaneAircraftHmiSenComponent::run([[maybe_unused]] sen::kernel::RunApi& api) {
  return api.execLoop(sen::Duration::fromHertz(1.0), [this]() { this->updateUIWithNewValues(); });
}

sen::kernel::FuncResult XplaneAircraftHmiSenComponent::unload([[maybe_unused]] sen::kernel::UnloadApi&& /*api*/) {
  pose_callbacks_.clear();
  x_plane_subscription_.reset();
  return done();
}

void XplaneAircraftHmiSenComponent::updateUIWithNewValues() {
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
