#include "sen/kernel/component.h"

#include "sen/kernel/component_api.h"
#include "sen/kernel/kernel.h"
#include <stl/hmi_sen_bridge/config.stl.h>

class SEN_EXPORT HmiSenBridge: public sen::kernel::Component
{
  public:
  HmiSenBridge()=default;
  sen::kernel::FuncResult load(sen::kernel::LoadApi&& api) override;

  sen::kernel::PassResult init(sen::kernel::InitApi&& /*api*/) override;

  sen::kernel::FuncResult run(sen::kernel::RunApi& api) override;

  sen::kernel::FuncResult unload(sen::kernel::UnloadApi&& /*api*/) override;

private:
  hmi_sen_bridge::Configuration config_;
};
