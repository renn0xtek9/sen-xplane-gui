#include "pluginmanager.h"

#include "plugininterface.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QVariantMap>

PluginManager::PluginManager(const QString& pluginDirectory, QObject* parent) : QObject(parent) {
  loadPlugins(pluginDirectory);
}

QVariantList PluginManager::plugins() const {
  return plugins_;
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

void PluginManager::setPanelVisible(const QString& pluginId, bool visible) {
  updatePluginProperty(pluginId, "panelVisible", visible);
}

void PluginManager::loadPlugins(const QString& pluginDirectory) {
  const QDir directory(pluginDirectory);
  const QFileInfoList files = directory.entryInfoList(QDir::Files);

  for (const QFileInfo& file : files) {
    if (!QLibrary::isLibrary(file.fileName())) {
      continue;
    }

    auto* loader = new QPluginLoader(file.absoluteFilePath(), this);
    QObject* instance = loader->instance();
    if (instance == nullptr) {
      qWarning() << "Could not load plugin" << file.fileName() << loader->errorString();
      delete loader;
      continue;
    }

    auto* plugin = qobject_cast<HmiPlugin*>(instance);
    if (plugin == nullptr) {
      qWarning() << "Plugin does not implement HmiPlugin:" << file.fileName();
      loader->unload();
      delete loader;
      continue;
    }

    QObject* backend = plugin->createBackend(instance);
    if (backend == nullptr) {
      qWarning() << "Plugin did not provide a backend:" << plugin->id();
      loader->unload();
      delete loader;
      continue;
    }

    plugins_.append(QVariantMap{
        {"pluginId", plugin->id()},
        {"pluginName", plugin->name()},
        {"panelTitle", plugin->panelTitle()},
        {"description", plugin->description()},
        {"panelSource", plugin->panelSource()},
        {"backend", QVariant::fromValue(backend)},
        {"pluginActive", true},
        {"panelVisible", true},
    });
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