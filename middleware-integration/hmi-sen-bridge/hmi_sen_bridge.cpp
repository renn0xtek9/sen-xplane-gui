#include "hmi_sen_bridge.h"

#include <chrono>

sen::kernel::FuncResult HmiSenBridge::run(sen::kernel::RunApi& api) {
  return api.execLoop(sen::Duration{std::chrono::milliseconds(250)}, []() {});
}

SEN_COMPONENT(HmiSenBridge)
