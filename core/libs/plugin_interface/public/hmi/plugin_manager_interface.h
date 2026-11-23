#pragma once
#include <QList>
#include <QObject>
#include <QString>
#include <QVariantList>

#include <memory>
#include <vector>

#include <hmi/plugin_factory_interface.h>
#include <hmi/plugin_interface.h>

namespace hmi {

class PluginManagerInterface {
public:
  virtual ~PluginManagerInterface() = default;
  [[nodiscard]] virtual QVariantList availableViews() const = 0;
  [[nodiscard]] virtual QVariantList instantiatedViews() const = 0;
  [[nodiscard]] virtual bool hasPlugin(const QString& pluginId) const = 0;
  [[nodiscard]] virtual std::vector<std::shared_ptr<Plugin>> getPlugins() const = 0;

  virtual void addFactory(const std::shared_ptr<PluginFactory>& factory) = 0;
  virtual void instantiateView(const QString& pluginId, const QString& viewId) = 0;
};

} // namespace hmi
