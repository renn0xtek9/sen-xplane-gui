#include <memory>

#include <hmi/plugin_factory_interface.h>
#include <hmi/plugin_interface.h>
#include <hmi/pluginmanager.h>

#include <gtest/gtest.h>
using namespace testing;

class PluginNoViewNoBackend : public hmi::Plugin {
public:
  [[nodiscard]] QString id() const final {
    return "pluginA";
  }
  [[nodiscard]] QString name() const final {
    return "Plugin A";
  }
  [[nodiscard]] QString description() const final {
    return "The plugin A";
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

TEST(PluginManager, InitializedWithNonExistingPath) {
  std::vector<std::shared_ptr<hmi::PluginFactory>> factories{};

  std::unique_ptr<hmi::PluginInstanceProviderInterface> dummyProvider = std::make_unique<DummyInstanceProvider>();

  hmi::PluginManager unit(std::move(dummyProvider), factories, nullptr);
}

TEST(PluginManager, PluginNoViewNoBackend) {
  auto factory = std::make_shared<PluginFactoryNoViewNoBackend>();

  std::vector<std::shared_ptr<hmi::PluginFactory>> factories{factory};

  std::unique_ptr<hmi::PluginInstanceProviderInterface> dummyProvider = std::make_unique<DummyInstanceProvider>();

  hmi::PluginManager unit(std::move(dummyProvider), factories, nullptr);
  EXPECT_EQ(unit.getPlugins().size(), 0U);
  EXPECT_EQ(unit.plugins().size(), 0U);
  EXPECT_EQ(unit.panels().size(), 0U);
  EXPECT_FALSE(unit.hasPlugin("pluginA"));
}
