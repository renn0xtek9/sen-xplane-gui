
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QObject>
#include <QPluginLoader>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QtCore/qcontainerfwd.h>
#include <QtCore/qtmetamacros.h>
#include <QtLogging>

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include <hmi/plugin_factory_interface.h>
#include <hmi/plugin_instance_provider_interface.h>
#include <hmi/plugin_interface.h>
#include <hmi/pluginmanager.h>

namespace hmi {

PluginManager::PluginManager(std::unique_ptr<PluginInstanceProviderInterface>&& instanceProvider,
                             std::vector<std::shared_ptr<PluginFactory>>& pluginFactories, QObject* parent)
    : QObject(parent), instance_provider_{std::move(instanceProvider)}, plugin_factories_(pluginFactories) {
  loadPlugins();
}

QVariantList PluginManager::instantiatedViews() const {
  return instantiatedViews_;
}

QVariantList PluginManager::availableViews() const {
  QVariantList availableViewsList;
  for (const auto& plugin : hmi_plugins_) {

    for (const auto& pluginView : plugin->views()) {
      QVariantMap availableView{};
      availableView.insert("pluginId", plugin->id());
      availableView.insert("pluginName", plugin->name());
      availableView.insert("viewId", pluginView.id);
      availableView.insert("viewTitle", pluginView.title);
      availableViewsList.append(availableView);
    }
  }
  return availableViewsList;
}

std::vector<std::shared_ptr<Plugin>> PluginManager::getPlugins() const {
  return hmi_plugins_;
}

bool PluginManager::hasPlugin(const QString& pluginId) const {
  return std::ranges::any_of(hmi_plugins_, [&pluginId](const auto& hmiPlugin) { return hmiPlugin->id() == pluginId; });
}

void PluginManager::tryLoadingPluginWithAnyFactory(QObject* instance) {
  for (auto& factory : plugin_factories_) {
    auto plugin = factory->createPlugin(instance);
    if (!plugin) {
      continue;
    }
    hmi_plugins_.push_back(plugin);
    return;
  }
}

void PluginManager::loadPlugins() {

  auto instances = instance_provider_->getPluginInstances();
  for (auto* instance : instances) {
    tryLoadingPluginWithAnyFactory(instance);
  }
  emit availableViewsChanged();
}

void PluginManager::instantiateView(const QString& pluginId, const QString& viewId) {
  auto pluginIt =
      std::ranges::find_if(hmi_plugins_, [&pluginId](const auto& plugin) { return pluginId == plugin->id(); });
  if (pluginIt == hmi_plugins_.end()) {
    return;
  }

  const auto pluginViews = (*pluginIt)->views();
  const auto pluginViewIt =
      std::ranges::find_if(pluginViews, [&viewId](const PluginView& pluginView) { return pluginView.id == viewId; });
  if (pluginViewIt == pluginViews.end()) {
    return;
  }

  auto* object = (*pluginIt)->instantiateView(viewId, *this);
  if (object == nullptr) {
    return;
  }

  QVariantMap instantiatedViewMap{};
  instantiatedViewMap.insert("viewControl", QVariant::fromValue(object));
  instantiatedViewMap.insert("viewSource", pluginViewIt->source);
  instantiatedViews_.append(instantiatedViewMap);
  emit instantiatedViewsChanged();
}

void PluginManager::addFactory(const std::shared_ptr<PluginFactory>& factory) {
  plugin_factories_.push_back(factory);
}
} // namespace hmi
