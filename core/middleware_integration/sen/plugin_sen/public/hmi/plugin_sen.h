#pragma once
#include <hmi/plugin_interface.h>

#include <sen/kernel/component.h>

#include <memory>

namespace hmi {

class PluginSen : public Plugin {
public:
  virtual ~PluginSen() = default;

  [[nodiscard]] virtual std::shared_ptr<sen::kernel::Component> getSenComponent() const = 0;
};
} // namespace hmi
#define PLUGIN_SEN_IID "org.senxplane.hmi.PluginSen/2.0"
Q_DECLARE_INTERFACE(hmi::PluginSen, PLUGIN_SEN_IID)
