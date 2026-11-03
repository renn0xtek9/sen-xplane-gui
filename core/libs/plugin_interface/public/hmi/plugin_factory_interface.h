#pragma once
#include <QObject>
#include <QString>

#include <memory>

#include <hmi/plugin_interface.h>

namespace hmi {

class PluginFactory {
public:
  virtual ~PluginFactory() = default;
  virtual std::shared_ptr<Plugin> createPlugin(QObject* instance) const = 0;
  virtual QString pluginType() const = 0;
};
} // namespace hmi
#define PLUGIN_FACTORY_IID "org.senxplane.hmi.PluginFactory/1.0"
Q_DECLARE_INTERFACE(hmi::PluginFactory, PLUGIN_FACTORY_IID)
