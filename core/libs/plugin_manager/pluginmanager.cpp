
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
#include <vector>

#include <hmi/plugin_factory_interface.h>
#include <hmi/plugin_interface.h>
#include <hmi/pluginmanager.h>

namespace hmi {

PluginManager::PluginManager(std::unique_ptr<PluginInstanceProviderInterface>&& instanceProvider,
                             std::vector<std::shared_ptr<PluginFactory>>& pluginFactories, QObject* parent)
    : instance_provider_{std::move(instanceProvider)}, plugin_factories_(pluginFactories), QObject(parent) {
  loadPlugins();
}

QVariantList PluginManager::plugins() const {
  return plugins_;
}

QVariantList PluginManager::panels() const {
  QVariantList panelEntries;
  for (const QVariant& pluginEntry : plugins_) {
    const QVariantMap plugin = pluginEntry.toMap();
    for (const QVariant& panelEntry : plugin.value("panels").toList()) {
      QVariantMap panel = panelEntry.toMap();
      panel.insert("pluginId", plugin.value("pluginId"));
      panel.insert("pluginName", plugin.value("pluginName"));
      panel.insert("pluginActive", plugin.value("pluginActive"));
      panel.insert("backend", plugin.value("backend"));
      panelEntries.append(panel);
    }
  }
  return panelEntries;
}

std::vector<std::shared_ptr<Plugin>> PluginManager::getPlugins() const {
  return hmi_plugins_;
}

bool PluginManager::hasPlugin(const QString& pluginId) const {
  return std::ranges::any_of(
      plugins_, [&pluginId](const QVariant& entry) { return entry.toMap().value("pluginId").toString() == pluginId; });
}

void PluginManager::setPluginActive(const QString& pluginId, bool active) {
  updatePluginProperty(pluginId, "pluginActive", active);
}

void PluginManager::setPanelVisible(const QString& pluginId, const QString& panelId, bool visible) {
  updatePanelProperty(pluginId, panelId, "panelVisible", visible);
}

void PluginManager::tryLoadingPluginWithAnyFactory(QObject* instance) {
  for (auto& factory : plugin_factories_) {
    auto plugin = factory->createPlugin(instance);
    if (!plugin) {
      continue;
    }
    auto* backend = plugin->createBackend(instance);
    if (backend == nullptr) {
      qWarning() << plugin->name() << " does not provide a backend";
      return;
    }
    hmi_plugins_.push_back(plugin);
    QVariantList panels;
    for (const PluginView& panel : plugin->views()) {
      panels.append(QVariantMap{
          {"panelId", panel.id},
          {"panelTitle", panel.title},
          {"panelSource", panel.source},
          {"panelVisible", true},
      });
    }

    plugins_.append(QVariantMap{
        {"pluginId", plugin->id()},
        {"pluginName", plugin->name()},
        {"description", plugin->description()},
        {"panels", panels},
        {"backend", QVariant::fromValue(backend)},
        {"pluginActive", true},
    });
    return;
  }
}

void PluginManager::loadPlugins() {

  auto instances = instance_provider_->getPluginInstances();
  for (auto* instance : instances) {
    tryLoadingPluginWithAnyFactory(instance);
  }
  emit pluginsChanged();
}

void PluginManager::updatePluginProperty(const QString& pluginId, const QString& property, const QVariant& value) {
  for (QVariant& pluginValue : plugins_) {
    QVariantMap plugin = pluginValue.toMap();
    if (plugin.value("pluginId").toString() != pluginId || plugin.value(property) == value) {
      continue;
    }

    plugin.insert(property, value);
    pluginValue = plugin;
    emit pluginsChanged();
    return;
  }
}

void PluginManager::updatePanelProperty(const QString& pluginId, const QString& panelId, const QString& property,
                                        const QVariant& value) {
  for (QVariant& pluginValue : plugins_) {
    QVariantMap plugin = pluginValue.toMap();
    if (plugin.value("pluginId").toString() != pluginId) {
      continue;
    }

    QVariantList panels = plugin.value("panels").toList();
    for (QVariant& panelValue : panels) {
      QVariantMap panel = panelValue.toMap();
      if (panel.value("panelId").toString() != panelId || panel.value(property) == value) {
        continue;
      }

      panel.insert(property, value);
      panelValue = panel;
      plugin.insert("panels", panels);
      pluginValue = plugin;
      emit pluginsChanged();
      return;
    }
  }
}

void PluginManager::addFactory(const std::shared_ptr<PluginFactory>& factory) {
  plugin_factories_.push_back(factory);
}
} // namespace hmi
