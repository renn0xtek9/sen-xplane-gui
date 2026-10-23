#pragma once
#include <hmi/plugin_interface.h>
#include <memory>

#include <QObject>

class HmiPluginFactory {
public:
  virtual ~HmiPluginFactory() = default;
  virtual std::shared_ptr<HmiPlugin> createPlugin(QObject* instance) const = 0;
  virtual QString pluginType() const = 0;
};

#define HMI_PLUGIN_FACTORY_IID "org.senxplane.hmi.HmiPluginFactory/1.0"
Q_DECLARE_INTERFACE(HmiPluginFactory, HMI_PLUGIN_FACTORY_IID)
