#pragma once

#include <QList>
#include <QObject>
#include <QString>
#include <QUrl>

struct HmiPluginPanel {
  QString id;
  QString title;
  QUrl source;
};

class HmiPlugin {
public:
  virtual ~HmiPlugin() = default;

  [[nodiscard]] virtual QString id() const = 0;
  [[nodiscard]] virtual QString name() const = 0;
  [[nodiscard]] virtual QString description() const = 0;
  [[nodiscard]] virtual QList<HmiPluginPanel> panels() const = 0;
  [[nodiscard]] virtual QObject* createBackend(QObject* parent) = 0;
};

#define HMI_PLUGIN_IID "org.senxplane.hmi.HmiPlugin/2.0"
Q_DECLARE_INTERFACE(HmiPlugin, HMI_PLUGIN_IID)
