#include <QObject>
#include <QUrl>
#include <Qt>

#include <memory>
#include <vector>

#include <hmi/plugin_factory_interface.h>
#include <hmi/pluginmanager.h>

#include "dummy_plugins.h"
#include <gtest/gtest.h>
using namespace testing;

TEST(PluginManager, InitializedWithNonExistingPath) {
  std::vector<std::shared_ptr<hmi::PluginFactory>> factories{};

  const hmi::PluginManager unit(std::make_unique<DummyInstanceProvider>(), factories, nullptr);
}

TEST(PluginManager, PluginNoViewNoBackend) {
  auto factory = std::make_shared<PluginFactoryNoViewNoBackend>();

  std::vector<std::shared_ptr<hmi::PluginFactory>> factories{factory};

  const hmi::PluginManager unit(std::make_unique<DummyInstanceProvider>(), factories, nullptr);
  EXPECT_EQ(unit.getPlugins().size(), 0U);
  EXPECT_EQ(unit.availableViews().size(), 0U);
  EXPECT_EQ(unit.instantiatedViews().size(), 0U);
  EXPECT_FALSE(unit.hasPlugin("PluginNoViewNoBackend"));
}

TEST(PluginManager, PluginWithSingleView) {
  auto factory = std::make_shared<PluginFactorySingleView>();

  std::vector<std::shared_ptr<hmi::PluginFactory>> factories{factory};

  hmi::PluginManager unit(std::make_unique<PluginSingleViewInstanceProvider>(), factories, nullptr);
  EXPECT_EQ(unit.getPlugins().size(), 1U);
  EXPECT_TRUE(unit.hasPlugin("PluginWithSingleView"));
  EXPECT_EQ(unit.availableViews().size(), 1U);
  EXPECT_EQ(unit.instantiatedViews().size(), 0U) << "Plugin manager shall not instantiate views by default";

  unit.instantiateView("PluginWithSingleView", "Some view");
  ASSERT_EQ(unit.instantiatedViews().size(), 1U);
  const auto instantiatedView = unit.instantiatedViews().first().toMap();
  EXPECT_EQ(instantiatedView.value("viewSource").toUrl(), QUrl("/some/source"));
  EXPECT_NE(instantiatedView.value("viewControl").value<QObject*>(), nullptr);

  unit.instantiateView("PluginWithSingleView", "Unknown view");
  unit.instantiateView("Unknown plugin", "Some view");
  EXPECT_EQ(unit.instantiatedViews().size(), 1);
}

// Add instantiate view to pluginmanager and write a test case for it.

// Test a plugin that returns a single view
//  - the instantiate view can be called
//  - it shall create the backend

// Test a plugin that returns a single view
//  - several view can be instantiated
//  - they have differente life
//  - the create backend is call several times (it is up to the plugin to do something about that)
