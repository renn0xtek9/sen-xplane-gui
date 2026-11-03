#include <QObject>
#include <QString>

#include <memory>

#include <hmi/plugin_interface.h>
#include <hmi/plugin_sen.h>
#include <hmi/plugin_sen_factory.h>

namespace hmi {

std::shared_ptr<Plugin> PluginSenFactory::createPlugin(QObject* instance) const {
  auto* plugin = qobject_cast<PluginSen*>(instance);
  return std::shared_ptr<PluginSen>(plugin);
}
QString PluginSenFactory::pluginType() const {
  return "PluginSen";
}
} // namespace hmi
