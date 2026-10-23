#include <hmi/plugin_interface.h>
#include <hmi/pluginmanager.h>

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QVariantMap>

PluginManager::PluginManager(const QString& pluginDirectory,
                             std::vector<std::shared_ptr<HmiPluginFactory>> pluginFactories, QObject* parent)
    : hmi_plugin_factories_(pluginFactories), QObject(parent) {
  loadPlugins(pluginDirectory);
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

std::vector<std::shared_ptr<HmiPlugin>> PluginManager::getPlugins() const {
  return hmi_plugins_;
}

bool PluginManager::hasPlugin(const QString& pluginId) const {
  for (const QVariant& entry : plugins_) {
    if (entry.toMap().value("pluginId").toString() == pluginId) {
      return true;
    }
  }

  return false;
}

void PluginManager::setPluginActive(const QString& pluginId, bool active) {
  updatePluginProperty(pluginId, "pluginActive", active);
}

void PluginManager::setPanelVisible(const QString& pluginId, const QString& panelId, bool visible) {
  updatePanelProperty(pluginId, panelId, "panelVisible", visible);
}

void PluginManager::loadPlugins(const QString& pluginDirectory) {
  const QDir directory(pluginDirectory);
  const QFileInfoList files = directory.entryInfoList(QDir::Files);

  for (const QFileInfo& file : files) {
    if (!QLibrary::isLibrary(file.fileName())) {
      continue;
    }
    qInfo() << " Processing plugin : " << file.absoluteFilePath();

    auto* loader = new QPluginLoader(file.absoluteFilePath(), this);
    QObject* instance = loader->instance();
    if (instance == nullptr) {
      qWarning() << "Could not load plugin" << file.fileName() << loader->errorString();
      delete loader;
      continue;
    }

    for (auto& factory : hmi_plugin_factories_) {
      qInfo() << "Trying to load plugin";
      auto plugin = factory->createPlugin(instance);
      if (plugin) {
        hmi_plugins_.push_back(plugin);
        qInfo() << "Created a new plugin instance for" << file.fileName() << "type" << factory->pluginType();
        QObject* backend = plugin->createBackend(instance);
        if (backend == nullptr) {
          qWarning() << "Plugin did not provide a backend:" << plugin->id();
          loader->unload();
          delete loader;
        } else {
          QVariantList panels;
          for (const HmiPluginPanel& panel : plugin->panels()) {
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
        }
        break;
      }

      // TODO if none is found
      //   qWarning() << "Plugin does not implement HmiPlugin:" << file.fileName();
      //  loader->unload();
      //  delete loader;
      //  continue;
    }
    loaders_.append(loader);
  }

  emit pluginsChanged();
}

void PluginManager::updatePluginProperty(const QString& pluginId, const QString& property, const QVariant& value) {
  for (qsizetype index = 0; index < plugins_.size(); ++index) {
    QVariantMap plugin = plugins_[index].toMap();
    if (plugin.value("pluginId").toString() != pluginId || plugin.value(property) == value) {
      continue;
    }

    plugin.insert(property, value);
    plugins_[index] = plugin;
    emit pluginsChanged();
    return;
  }
}

void PluginManager::updatePanelProperty(const QString& pluginId, const QString& panelId, const QString& property,
                                        const QVariant& value) {
  for (qsizetype pluginIndex = 0; pluginIndex < plugins_.size(); ++pluginIndex) {
    QVariantMap plugin = plugins_[pluginIndex].toMap();
    if (plugin.value("pluginId").toString() != pluginId) {
      continue;
    }

    QVariantList panels = plugin.value("panels").toList();
    for (qsizetype panelIndex = 0; panelIndex < panels.size(); ++panelIndex) {
      QVariantMap panel = panels[panelIndex].toMap();
      if (panel.value("panelId").toString() != panelId || panel.value(property) == value) {
        continue;
      }

      panel.insert(property, value);
      panels[panelIndex] = panel;
      plugin.insert("panels", panels);
      plugins_[pluginIndex] = plugin;
      emit pluginsChanged();
      return;
    }
  }
}

void PluginManager::addFactory(std::shared_ptr<HmiPluginFactory> factory) {
  hmi_plugin_factories_.push_back(factory);
}
