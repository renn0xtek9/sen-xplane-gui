#pragma once
#include <sen/core/base/compiler_macros.h>
#include <sen/core/meta/var.h>
#include <sen/kernel/component_api.h>

#include <stl/hmi/hmi_handle.stl.h>

namespace hmi_handle {
class SEN_EXPORT HmiHandleImpl : public hmi::HmiHandleBase {

  SEN_NOCOPY_NOMOVE(HmiHandleImpl)

public:
  HmiHandleImpl(std::string name, const sen::VarMap& args) : hmi::HmiHandleBase(name) {
    std::ignore = args;
    std::cout << "Handle created" << std::endl;
  }
  ~HmiHandleImpl() = default;

protected:
  void registered(sen::kernel::RegistrationApi& api) override;
  void unregistered(sen::kernel::RegistrationApi& api) override;

  void minimizeImpl() override;
  void fullScreenImpl() override;
};

} // namespace hmi_handle
