#include "x_plane_node.h"

#include <cmath>
#include <numbers>
#include <string>
#include <utility>

#include <sen/core/meta/class_type.h>
#include <sen/core/meta/var.h>
#include <sen/kernel/component_api.h>

#include <stl/x_plane_node/x_plane_node.stl.h>
namespace x_plane_node {

XPlaneNodeImpl::XPlaneNodeImpl(const std::string& name, const sen::VarMap& args) : XPlaneNodeBase(name, args) {}

void XPlaneNodeImpl::update(sen::kernel::RunApi& runApi) {
  std::ignore = runApi;
  // TODO retrieve actual telemtry data from x-plane
  // See also https://developer.x-plane.com/sdk/XPLMDataAccess
  const auto elapsedMilliseconds = static_cast<double>(counter_) * 1000.0 / 30.0;
  const double phase = 2.0 * std::numbers::pi * elapsedMilliseconds;
  auto pose = getPose();
  pose.elevationAboveMsl = static_cast<float>(1000.0 + (10.0 * std::sin(phase / 10000.0)));
  pose.groundSpeed = static_cast<float>(200.0 + (10.0 * std::sin(phase / 5000.0)));
  setNextPose(pose);
  counter_++;
}

SEN_EXPORT_CLASS(XPlaneNodeImpl)

} // namespace x_plane_node
