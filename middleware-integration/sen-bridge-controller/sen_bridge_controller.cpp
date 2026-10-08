#include "sen_bridge_controller.h"

#include <memory>
#include <stl/sen/kernel/basic_types.stl.h>
#include <thread>

#include <QDebug>

#include <sen/kernel/bootloader.h>
#include <sen/kernel/kernel.h>

#include <hmi_sen_bridge/hmi_sen_bridge.h>

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
  appName: hmi_sen_bridge
  bus: hmi.mybus
load:
  - name: ether
    group: 1
)";
    auto bootloader = sen::kernel::Bootloader::fromYamlString(bootConfig, false);
    auto& config = bootloader->getConfig();
    auto* component = new HmiSenBridge();

    sen::kernel::KernelConfig::ComponentToLoad bridge;

    bridge.component.instance = component;
    bridge.component.info.name = "hmi_sen_bridge";
    bridge.config.group = 4;
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

#include "sen_bridge_controller.moc"
