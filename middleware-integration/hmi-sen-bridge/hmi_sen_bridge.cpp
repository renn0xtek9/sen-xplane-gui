#include "hmi_sen_bridge.h"

#include <iostream>
#include <chrono>

sen::kernel::FuncResult HmiSenBridge::run(sen::kernel::RunApi& api) {
  std::cout<<"Running !!!"<<std::endl;
  return api.execLoop(sen::Duration{std::chrono::milliseconds(250)}, []() {
    std::cout<<"HMISenBridge running"<<std::endl;
  });
}

SEN_COMPONENT(HmiSenBridge)
