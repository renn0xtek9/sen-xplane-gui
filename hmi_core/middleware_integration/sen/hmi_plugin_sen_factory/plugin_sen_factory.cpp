#include <hmi/plugin_sen.h>
#include <hmi/plugin_sen_factory.h>

std::shared_ptr<HmiPlugin> HmiPluginSenFactory::createPlugin(QObject* instance) const {
  auto plugin = qobject_cast<HmiPluginSen*>(instance);
  return std::shared_ptr<HmiPluginSen>(plugin);
}
QString HmiPluginSenFactory::pluginType() const {
  return "HmiPluginSen";
}
