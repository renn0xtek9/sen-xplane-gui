#include <sen/kernel/bootloader.h>
#include <sen/kernel/kernel.h>
#include <sen/kernel/kernel_config.h>

#include <x_plane_aircraft_hmi/sen_component.h>

#include <iostream>
#include <utility>

int main(int argc, char* argv[]) {
  std::ignore = argc;
  std::ignore = argv;
  constexpr auto bootConfig = R"(
kernel:
  appName: hmi_sen_component
  bus: hmi.mybus
load:
  - name: ether
    group: 1
)";
  auto bootloader = sen::kernel::Bootloader::fromYamlString(bootConfig, false);
  auto& config = bootloader->getConfig();
  auto* component = new XplaneAircraftHmiSenComponent();

  sen::kernel::KernelConfig::ComponentToLoad componentToLoad;

  componentToLoad.component.instance = component;
  componentToLoad.component.info.name = "test_component";
  componentToLoad.config.group = 4;
  config.addToLoad(componentToLoad);
  sen::kernel::Kernel kernel(config);
  const int runResult = kernel.run(sen::kernel::KernelBlockMode::doNotBlock);
  std::ignore = runResult;
  std::cout << "Kernel running. Press key to stop\n";
  char c{};
  std::cin >> c;
  kernel.requestStop();

  std::cout << "Test success !";
}
