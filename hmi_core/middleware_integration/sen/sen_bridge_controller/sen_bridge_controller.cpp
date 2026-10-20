#include <QDebug>

#include <sen/kernel/bootloader.h>
#include <sen/kernel/component_api.h>
#include <sen/kernel/kernel.h>

#include <stl/sen/kernel/basic_types.stl.h>

#include <hmi/hmi_sen_component.h>
#include <hmi/plugin_sen.h>
#include <hmi/sen_bridge_controller.h>

#include <memory>
#include <thread>

class SenBridgeController::SenBridgeControllerPrivate {
public:
  explicit SenBridgeControllerPrivate(SenBridgeController* q) : q(q) {}

  bool connected = false;
  std::unique_ptr<sen::kernel::Kernel> kernel;
  std::thread bridgeThread;

  SenBridgeController* q;

  void stop() {
    if (kernel != nullptr) {
      kernel->requestStop();
      kernel.reset();
    }

    if (bridgeThread.joinable()) {
      bridgeThread.join();
    }
  }
};

SenBridgeController::SenBridgeController(QObject* parent)
    : QObject(parent), d(std::make_unique<SenBridgeControllerPrivate>(this)) {}

SenBridgeController::~SenBridgeController() {
  stop();
}

bool SenBridgeController::connected() const {
  return d->connected;
}

void SenBridgeController::connectToSen() {
  if (d->connected || d->bridgeThread.joinable()) {
    qInfo() << "Ignoring duplicate Sen connection request";
    return;
  }

  qInfo() << "Starting Sen kernel";
  d->bridgeThread = std::thread([this]() {
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
    auto* component = new HmiSenComponent();

    sen::kernel::KernelConfig::ComponentToLoad bridge;

    bridge.component.instance = component;
    bridge.component.info.name = "hmi_sen_component";
    bridge.config.group = 4U;

    for (auto& plugin : m_plugins) {
      sen::kernel::KernelConfig::ComponentToLoad componentToLoad;
      componentToLoad.component.instance = plugin->getSenComponent().get();
      componentToLoad.component.info.name = plugin->name().toStdString();
      componentToLoad.component.config.group = 4U;
      componentToLoad.config.group = 4U;
      config.addToLoad(componentToLoad);
      qInfo() << "Added " << plugin->name() << " as component to load";
    }
    config.addToLoad(bridge);

    d->kernel = std::make_unique<sen::kernel::Kernel>(config);
    const int runResult = d->kernel->run(sen::kernel::KernelBlockMode::doNotBlock);

    qInfo() << "Sen kernel startup returned" << runResult;
    if (runResult != 0) {
      qWarning() << "Sen kernel failed to start";
      d->connected = false;
      emit connectedChanged();
    }
  });

  d->connected = true;
  emit connectedChanged();
  emit connectionEstablished();
}

void SenBridgeController::stop() {
  d->stop();
}

void SenBridgeController::addPlugins(std::vector<std::shared_ptr<HmiPlugin>> plugins) {
  for (auto& plugin : plugins) {
    std::shared_ptr<HmiPluginSen> hmi_sen_plugin = std::dynamic_pointer_cast<HmiPluginSen>(plugin);
    if (hmi_sen_plugin) {
      qInfo() << "Adding plugin: " << hmi_sen_plugin->name();
      m_plugins.push_back(hmi_sen_plugin);
    }
  }
}

// #include "sen_bridge_controller.moc"
