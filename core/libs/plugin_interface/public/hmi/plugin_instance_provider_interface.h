#pragma once
#include <QObject>
#include <QVector>

namespace hmi {
class PluginInstanceProviderInterface {
public:
  virtual ~PluginInstanceProviderInterface() = default;
  [[nodiscard]] virtual QVector<QObject*> getPluginInstances() = 0;
};
} // namespace hmi
