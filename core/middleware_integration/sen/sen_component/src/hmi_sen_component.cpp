#include <QDebug>

#include <hmi/hmi_handle.h>
#include <hmi/hmi_sen_component.h>

#include <sen/core/meta/var.h>
#include <sen/kernel/component.h>

#include <stl/hmi/config.stl.h>
#include <stl/sen/kernel/basic_types.stl.h>

sen::kernel::FuncResult HmiSenComponent::load(sen::kernel::LoadApi&& api) {
  // get our configuration
  config_ = sen::toValue<hmi::Configuration>(api.getConfig());
  std::cout << "HmiSenComponent config:\n" << config_ << "\n";
  // qInfo()<< "HmiSenComponent config:\n" << config_ ;
  return done();
}

sen::kernel::PassResult HmiSenComponent::init(sen::kernel::InitApi&& /*api*/) {
  qInfo() << "HmiSenComponent init";
  return done();
}

sen::kernel::FuncResult HmiSenComponent::run(sen::kernel::RunApi& api) {
  qInfo() << "HmiSenComponent run";
  const sen::kernel::BusAddress targetBus{"hmi", "mybus"};
  auto source = api.getSource(targetBus);

  auto handle = std::make_shared<hmi_handle::HmiHandleImpl>(std::string{"my-handle"}, sen::VarMap{});
  source->add(handle);

  return api.execLoop(sen::Duration::fromHertz(1.0),
                      [&source, &handle]() { std::cout << "HmiSenComponent run iteration\n"; });
}

sen::kernel::FuncResult HmiSenComponent::unload(sen::kernel::UnloadApi&& /*api*/) {
  qInfo() << "HmiSenComponent unload";
  return done();
}

SEN_COMPONENT(HmiSenComponent)
