#pragma once

#include <sen/kernel/component.h>

class HmiSenBridge final : public sen::kernel::Component {
public:
  HmiSenBridge() = default;
  ~HmiSenBridge() override = default;

  sen::kernel::FuncResult run(sen::kernel::RunApi& api) override;
};
