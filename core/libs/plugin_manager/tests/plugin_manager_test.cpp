#include <memory>

#include <hmi/plugin_factory_interface.h>
#include <hmi/pluginmanager.h>

#include <gtest/gtest.h>
using namespace testing;

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
