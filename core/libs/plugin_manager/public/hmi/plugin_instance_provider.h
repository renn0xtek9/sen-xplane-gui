#pragma once

#include <QList>
#include <QObject>
#include <QPluginLoader>
#include <QString>
#include <QVector>

#include <hmi/plugin_instance_provider_interface.h>
namespace hmi {

class PluginInstanceProvider : public PluginInstanceProviderInterface {
public:
  PluginInstanceProvider(QString pluginDirectory);
  ~PluginInstanceProvider() override = default;

  [[nodiscard]] QVector<QObject*> getPluginInstances() final;

private:
  QString plugin_directory_;
  QList<QPluginLoader*> loaders_;
};

} // namespace hmi
