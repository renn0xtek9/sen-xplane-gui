#include <sen/core/meta/class_type.h>

#include <hmi/hmi_handle.h>

namespace hmi_handle {

void HmiHandleImpl::registered(sen::kernel::RegistrationApi& api) {
  std::ignore = api;
}

void HmiHandleImpl::unregistered(sen::kernel::RegistrationApi& api) {
  std::ignore = api;
}

void HmiHandleImpl::minimizeImpl() {}

void HmiHandleImpl::fullScreenImpl() {}

SEN_EXPORT_CLASS(HmiHandleImpl)

} // namespace hmi_handle
