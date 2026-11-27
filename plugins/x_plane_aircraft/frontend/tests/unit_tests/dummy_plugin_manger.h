#pragma once
#include <QPair>
#include <QString>
#include <QVariantList>
#include <QVector>

#include <memory>
#include <vector>

#include <hmi/plugin_factory_interface.h>
#include <hmi/plugin_interface.h>
#include <hmi/plugin_manager_interface.h>

namespace hmi {

class DummyPluginFactory : public PluginFactory {
public:
  virtual ~DummyPluginFactory() = default;
  std::shared_ptr<Plugin> createPlugin(QObject* instance) const override {
    return {};
  }
  [[nodiscard]] QString pluginType() const override {
    return {};
  }
};

class DummyPluginManager : public PluginManagerInterface {
public:
  [[nodiscard]] QVariantList availableViews() const override {
    return {};
  }
  [[nodiscard]] QVariantList instantiatedViews() const override {
    return {};
  }
  [[nodiscard]] bool hasPlugin(const QString& pluginId) const override {
    return true;
  }
  [[nodiscard]] std::vector<std::shared_ptr<Plugin>> getPlugins() const override {
    return {};
  }

  void addFactory(const std::shared_ptr<PluginFactory>& factory) override {}
  void instantiateView(const QString& pluginId, const QString& viewId) override {
    instantiatedViewsCallArgs_.append(qMakePair(pluginId, viewId));
  }

  QVector<QPair<QString, QString>> instantiateViewCallArgs() {
    return instantiatedViewsCallArgs_;
  }

private:
  QVector<QPair<QString, QString>> instantiatedViewsCallArgs_;
};

} // namespace hmi
