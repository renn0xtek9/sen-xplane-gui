#include "sen/kernel/component.h"
#include <hmi-sen-bridge/hmi_sen_bridge.h>
#include <stl/hmi_sen_bridge/config.stl.h>
#include <hmi-sen-bridge/hmi_handle.h>
#include <QDebug>
#include <stl/sen/kernel/basic_types.stl.h>

  sen::kernel::FuncResult HmiSenBridge::load(sen::kernel::LoadApi&& api)   {
    // get our configuration
    config_ = sen::toValue<hmi_sen_bridge::Configuration>(api.getConfig());
    std::cout << "HmiSenBridge config:\n" << config_ << "\n";
    return done();
  }

  sen::kernel::PassResult HmiSenBridge::init(sen::kernel::InitApi&& /*api*/)   {
    qInfo() << "HmiSenBridge init";
    return done();
  }

  sen::kernel::FuncResult HmiSenBridge::run(sen::kernel::RunApi& api) 
  {
    qInfo() << "HmiSenBridge run";
    std::cout << "HmiSenBridge started running\n";fflush(stdout);
    const sen::kernel::BusAddress targetBus("hmi.hmibus");
    auto source=api.getSource(targetBus);


    auto handle= std::make_shared<hmi_sen_bridge::HmiHandleImpl>(std::string{"my-handle"});
    source->add(handle);

    return api.execLoop(sen::Duration::fromHertz(1.0), [&source](){
      std::cout << "HmiSenBridge run iteration\n";
    });
  }

  sen::kernel::FuncResult HmiSenBridge::unload(sen::kernel::UnloadApi&& /*api*/) 
  {
    qInfo() << "HmiSenBridge unload";
    std::cout << "HmiSenBridge unloaded\n";
    return done();
  }



SEN_COMPONENT(HmiSenBridge)
