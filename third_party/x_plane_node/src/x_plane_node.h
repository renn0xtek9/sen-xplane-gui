#pragma once

#include "stl/x_plane_node/x_plane_node.stl.h"

// sen
#include "sen/kernel/component_api.h"

namespace x_plane_node {

class XPlaneNodeImpl : public XPlaneNodeBase {
public:
  SEN_NOCOPY_NOMOVE(XPlaneNodeImpl)

public:
  XPlaneNodeImpl(const std::string& name, const sen::VarMap& args);
  ~XPlaneNodeImpl() override = default;

public:
  void update(sen::kernel::RunApi& runApi) override;

protected:
  int32_t counter_{};
};

} // namespace x_plane_node
