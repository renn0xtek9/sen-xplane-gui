#pragma once
#include <hmi/plugin_factory_interface.h>
#include <hmi/plugin_interface.h>

#include <QObject>
#include <QPluginLoader>
#include <QVariantList>

class PluginManager final : public QObject {
  Q_OBJECT
  Q_PROPERTY(QVariantList plugins READ plugins NOTIFY pluginsChanged)

public:
  explicit PluginManager(const QString& pluginDirectory, std::vector<std::shared_ptr<HmiPluginFactory>> pluginFactories,
                         QObject* parent = nullptr);

  [[nodiscard]] QVariantList plugins() const;
  [[nodiscard]] bool hasPlugin(const QString& pluginId) const;
  [[nodiscard]] std::vector<std::shared_ptr<HmiPlugin>> getPlugins() const;

  void addFactory(std::shared_ptr<HmiPluginFactory> factory);

  Q_INVOKABLE void setPluginActive(const QString& pluginId, bool active);
  Q_INVOKABLE void setPanelVisible(const QString& pluginId, bool visible);

signals:
  void pluginsChanged();

private:
  void loadPlugins(const QString& pluginDirectory);
  void updatePluginProperty(const QString& pluginId, const QString& property, const QVariant& value);

  QVariantList plugins_;
  QList<QPluginLoader*> loaders_;
  std::vector<std::shared_ptr<HmiPlugin>> hmi_plugins_;
  std::vector<std::shared_ptr<HmiPluginFactory>> hmi_plugin_factories_;
};
