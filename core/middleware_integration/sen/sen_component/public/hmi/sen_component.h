#pragma once
#include <sen/kernel/component.h>
#include <sen/kernel/component_api.h>
#include <sen/kernel/kernel.h>

#include <stl/hmi/config.stl.h>

class SEN_EXPORT HmiSenComponent : public sen::kernel::Component {
public:
  HmiSenComponent() = default;
  sen::kernel::FuncResult load([[maybe_unused]] sen::kernel::LoadApi&& api) override;

  sen::kernel::PassResult init([[maybe_unused]] sen::kernel::InitApi&& /*api*/) override;

  sen::kernel::FuncResult run([[maybe_unused]] sen::kernel::RunApi& api) override;

  sen::kernel::FuncResult unload([[maybe_unused]] sen::kernel::UnloadApi&& /*api*/) override;

private:
  hmi::Configuration config_;
};
