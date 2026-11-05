#pragma once
#include <memory>

#include <hmi/plugin_factory_interface.h>
#include <hmi/plugin_instance_provider_interface.h>
#include <hmi/plugin_interface.h>
// #include <hmi/pluginmanager.h>

#include "dummy_frontend.h"

class PluginNoViewNoBackend : public hmi::Plugin {
public:
  [[nodiscard]] QString id() const final {
    return "PluginNoViewNoBackend";
  }
  [[nodiscard]] QString name() const final {
    return "PluginNoViewNoBackend";
  }
  [[nodiscard]] QString description() const final {
    return "A Plugin with no view and no backend";
  }
  [[nodiscard]] QList<hmi::PluginView> views() const final {
    return {};
  }
  [[nodiscard]] QObject* createBackend(QObject* parent) final {
    return nullptr;
  }
};

class PluginFactoryNoViewNoBackend : public hmi::PluginFactory {
public:
  ~PluginFactoryNoViewNoBackend() override = default;
  std::shared_ptr<hmi::Plugin> createPlugin(QObject* instance) const final {
    return std::make_shared<PluginNoViewNoBackend>();
  }
  [[nodiscard]] QString pluginType() const final {
    return {"PluginNoViewNoBackend"};
  }
};

class DummyInstanceProvider : public hmi::PluginInstanceProviderInterface {
public:
  virtual ~DummyInstanceProvider() = default;
  [[nodiscard]] QVector<QObject*> getPluginInstances() final {
    return {};
  }
};

class PluginWithSingleView : public QObject, public hmi::Plugin {
  Q_OBJECT
public:
  explicit PluginWithSingleView(QObject* parent = nullptr) {}
  ~PluginWithSingleView() = default;

  [[nodiscard]] QString id() const final {
    return "PluginWithSingleView";
  }
  [[nodiscard]] QString name() const final {
    return "PluginWithSingleView";
  }
  [[nodiscard]] QString description() const final {
    return "A Plugin with no view and no backend";
  }
  [[nodiscard]] QList<hmi::PluginView> views() const final {
    hmi::PluginView myview;
    myview.id = "Some view";
    myview.title = "Some view title";
    myview.source = QUrl("/some/source");
    return {myview};
  }
  [[nodiscard]] QObject* createBackend(QObject* parent) final {
    return new DummyFrontend(parent);
  }
};

class PluginFactorySingleView : public hmi::PluginFactory {
public:
  ~PluginFactorySingleView() override = default;
  std::shared_ptr<hmi::Plugin> createPlugin(QObject* instance) const final {
    auto* plugin = qobject_cast<PluginWithSingleView*>(instance);
    return std::shared_ptr<PluginWithSingleView>(plugin);
  }
  [[nodiscard]] QString pluginType() const final {
    return {"PluginWithSingleView"};
  }
};

class PluginSingleViewInstanceProvider : public hmi::PluginInstanceProviderInterface {
public:
  virtual ~PluginSingleViewInstanceProvider() = default;
  [[nodiscard]] QVector<QObject*> getPluginInstances() final {
    return {new PluginWithSingleView(nullptr)};
  }
};
