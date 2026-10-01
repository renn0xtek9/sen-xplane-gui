#pragma once

#include <QObject>
#include <QPluginLoader>
#include <QVariantList>

class PluginManager final : public QObject {
  Q_OBJECT
  Q_PROPERTY(QVariantList plugins READ plugins NOTIFY pluginsChanged)

public:
  explicit PluginManager(const QString &pluginDirectory,
                         QObject *parent = nullptr);

  [[nodiscard]] QVariantList plugins() const;
  [[nodiscard]] bool hasPlugin(const QString &pluginId) const;

  Q_INVOKABLE void setPluginActive(const QString &pluginId, bool active);
  Q_INVOKABLE void setPanelVisible(const QString &pluginId, bool visible);

signals:
  void pluginsChanged();

private:
  void loadPlugins(const QString &pluginDirectory);
  void updatePluginProperty(const QString &pluginId, const QString &property,
                            const QVariant &value);

  QVariantList plugins_;
  QList<QPluginLoader *> loaders_;
};
