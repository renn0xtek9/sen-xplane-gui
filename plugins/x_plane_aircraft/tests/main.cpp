#include <sen/kernel/bootloader.h>
#include <sen/kernel/kernel.h>

#include <x_plane_aircraft_hmi/sen_component.h>

int main(int argc, char* argv[]) {

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
  auto component = new XplaneAircraftHmiSenComponent();

  sen::kernel::KernelConfig::ComponentToLoad componentToLoad;

  componentToLoad.component.instance = component;
  componentToLoad.component.info.name = "test_component";
  componentToLoad.config.group = 4;
  config.addToLoad(componentToLoad);
  sen::kernel::Kernel kernel(config);
  const int runResult = kernel.run(sen::kernel::KernelBlockMode::doNotBlock);

  std::cout << "Kernel running. Press key to stop" << std::endl;
  char c;
  std::cin >> c;
  kernel.requestStop();

  std::cout << "Test success !";
}
