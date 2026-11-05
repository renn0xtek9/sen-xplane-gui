#include <memory>

#include <hmi/plugin_factory_interface.h>
#include <hmi/plugin_interface.h>
#include <hmi/pluginmanager.h>

#include "dummy_plugins.h"
#include <gtest/gtest.h>
using namespace testing;

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
  EXPECT_FALSE(unit.hasPlugin("PluginNoViewNoBackend"));
}

TEST(PluginManager, PluginWithSingleView) {
  auto factory = std::make_shared<PluginFactorySingleView>();

  std::vector<std::shared_ptr<hmi::PluginFactory>> factories{factory};

  std::unique_ptr<hmi::PluginInstanceProviderInterface> dummyProvider =
      std::make_unique<PluginSingleViewInstanceProvider>();

  hmi::PluginManager unit(std::move(dummyProvider), factories, nullptr);
  EXPECT_EQ(unit.getPlugins().size(), 1U);
  EXPECT_EQ(unit.plugins().size(), 1U);
  EXPECT_EQ(unit.panels().size(), 1U);
  EXPECT_TRUE(unit.hasPlugin("PluginWithSingleView"));
}

// Test a plugin that returns a single view
//  - the instantiate view can be called
//  - it shall create the backend

// Test a plugin that returns a single view
//  - several view can be instantiated
//  - they have differente life
//  - the create backend is call several times (it is up to the plugin to do something about that)
