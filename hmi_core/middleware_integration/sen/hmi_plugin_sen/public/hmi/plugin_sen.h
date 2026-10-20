#pragma once
#include <hmi/plugin_interface.h>

#include <sen/kernel/component.h>

#include <memory>

class HmiPluginSen : public HmiPlugin {
public:
  virtual ~HmiPluginSen() = default;

  [[nodiscard]] virtual std::shared_ptr<sen::kernel::Component> getSenComponent() const = 0;
};

#define HMI_PLUGIN_SEN_IID "org.senxplane.hmi.HmiPluginSen/1.0"
Q_DECLARE_INTERFACE(HmiPluginSen, HMI_PLUGIN_SEN_IID)
