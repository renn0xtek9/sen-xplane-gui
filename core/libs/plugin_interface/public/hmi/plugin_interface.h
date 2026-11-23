#pragma once

#include <QList>
#include <QObject>
#include <QString>
#include <QUrl>

namespace hmi {

struct PluginView {
  QString id;
  QString title;
  QUrl source;
};

class PluginManagerInterface;

class Plugin {
public:
  virtual ~Plugin() = default;

  [[nodiscard]] virtual QString id() const = 0;
  [[nodiscard]] virtual QString name() const = 0;
  [[nodiscard]] virtual QString description() const = 0;
  [[nodiscard]] virtual QList<PluginView> views() const = 0;
  [[nodiscard]] virtual QObject* instantiateView(const QString viewId, PluginManagerInterface& pluginManager) = 0;
};

} // namespace hmi

#define PLUGIN_IID "org.senxplane.hmi.Plugin/2.0"
Q_DECLARE_INTERFACE(hmi::Plugin, PLUGIN_IID)
