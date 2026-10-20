#pragma once
#include <hmi/plugin_factory_interface.h>

class HmiPluginSenFactory : public HmiPluginFactory {
public:
  virtual ~HmiPluginSenFactory() = default;
  virtual std::shared_ptr<HmiPlugin> createPlugin(QObject* instance) const;
  virtual QString pluginType() const;
};

#define HMI_PLUGIN_SEN_FACTORY_IID "org.senxplane.hmi.HmiPluginSenFactory/1.0"
Q_DECLARE_INTERFACE(HmiPluginSenFactory, HMI_PLUGIN_SEN_FACTORY_IID)
