#pragma once
#include <QObject>
#include <QVariantList>

#include <memory>

#include <hmi/plugin_factory_interface.h>
#include <hmi/plugin_instance_provider_interface.h>
#include <hmi/plugin_interface.h>

namespace hmi {

class PluginManager final : public QObject {
  Q_OBJECT
  Q_PROPERTY(QVariantList availableViews READ availableViews NOTIFY availableViewsChanged)
  Q_PROPERTY(QVariantList instantiatedViews READ instantiatedViews NOTIFY instantiatedViewsChanged)

public:
  explicit PluginManager(std::unique_ptr<PluginInstanceProviderInterface>&& instanceProvider,
                         std::vector<std::shared_ptr<PluginFactory>>& pluginFactories, QObject* parent = nullptr);

  [[nodiscard]] QVariantList availableViews() const;
  [[nodiscard]] QVariantList instantiatedViews() const;
  [[nodiscard]] bool hasPlugin(const QString& pluginId) const;
  [[nodiscard]] std::vector<std::shared_ptr<Plugin>> getPlugins() const;

  void addFactory(const std::shared_ptr<PluginFactory>& factory);

  Q_INVOKABLE void instantiateView(const QString& pluginId, const QString& viewId);

signals:
  void availableViewsChanged();
  void instantiatedViewsChanged();

private:
  void loadPlugins();
  void tryLoadingPluginWithAnyFactory(QObject* instance);

  std::vector<std::shared_ptr<Plugin>> hmi_plugins_;
  std::vector<std::shared_ptr<PluginFactory>> plugin_factories_;

  std::unique_ptr<PluginInstanceProviderInterface> instance_provider_;

  QVariantList instantiatedViews_;
};
} // namespace hmi
