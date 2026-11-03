#pragma once
#include <hmi/plugin_factory_interface.h>
namespace hmi {

class PluginSenFactory : public PluginFactory {
public:
  virtual ~PluginSenFactory() = default;
  virtual std::shared_ptr<Plugin> createPlugin(QObject* instance) const;
  virtual QString pluginType() const;
};
} // namespace hmi
#define PLUGIN_SEN_FACTORY_IID "org.senxplane.hmi.PluginSenFactory/1.0"
Q_DECLARE_INTERFACE(hmi::PluginSenFactory, PLUGIN_SEN_FACTORY_IID)
