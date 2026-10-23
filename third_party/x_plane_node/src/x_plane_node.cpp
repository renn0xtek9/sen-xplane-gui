#include "x_plane_node.h"

namespace x_plane_node {

XPlaneNodeImpl::XPlaneNodeImpl(const std::string& name, const sen::VarMap& args) : XPlaneNodeBase(name, args) {}

void XPlaneNodeImpl::update(sen::kernel::RunApi& runApi) {
  std::ignore = runApi; // here goes your update logic
  // TODO retrieve actual telemtry data from x-plane
  // See also https://developer.x-plane.com/sdk/XPLMDataAccess
  const double elapsedMilliseconds = static_cast<double>(counter_ * 1000 / 30);
  const double phase = 2.0 * 3.1415 * elapsedMilliseconds;
  auto pose = getPose();
  pose.elevationAboveMsl = 1000.0 + 10.0 * std::sin(phase / 10000.0);
  pose.groundSpeed = 200.0 + 10.0 * std::sin(phase / 5000.0);
  setNextPose(pose);
  counter_++;
}

SEN_EXPORT_CLASS(XPlaneNodeImpl)

} // namespace x_plane_node
