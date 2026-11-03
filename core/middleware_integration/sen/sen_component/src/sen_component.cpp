#include <memory>

#include <QDebug>

#include <hmi/hmi_handle.h>
#include <hmi/sen_component.h>

#include <sen/core/io/util.h>
#include <sen/core/meta/var.h>
#include <sen/kernel/component.h>
#include <sen/kernel/component_api.h>

#include <stl/hmi/config.stl.h>
#include <stl/sen/kernel/basic_types.stl.h>

// NOLINTNEXTLINE(cppcoreguidelines-rvalue-reference-param-not-moved): Required non-movable Sen lifecycle API.
sen::kernel::FuncResult HmiSenComponent::load([[maybe_unused]] sen::kernel::LoadApi&& api) {
  config_ = sen::toValue<hmi::Configuration>(api.getConfig());
  return done();
}

sen::kernel::PassResult HmiSenComponent::init([[maybe_unused]] sen::kernel::InitApi&& /*api*/) {
  return done();
}

sen::kernel::FuncResult HmiSenComponent::run([[maybe_unused]] sen::kernel::RunApi& api) {
  const sen::kernel::BusAddress targetBus{.sessionName = "hmi", .busName = "mybus"};
  auto source = api.getSource(targetBus);

  auto handle = std::make_shared<hmi_handle::HmiHandleImpl>(std::string{"my-handle"}, sen::VarMap{});
  source->add(handle);

  return api.execLoop(sen::Duration::fromHertz(1.0), []() {});
}

sen::kernel::FuncResult HmiSenComponent::unload([[maybe_unused]] sen::kernel::UnloadApi&& /*api*/) {
  return done();
}

SEN_COMPONENT(HmiSenComponent)
