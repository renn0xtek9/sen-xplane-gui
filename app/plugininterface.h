#pragma once

#include <QObject>
#include <QString>
#include <QUrl>

class HmiPlugin {
public:
  virtual ~HmiPlugin() = default;

  [[nodiscard]] virtual QString id() const = 0;
  [[nodiscard]] virtual QString name() const = 0;
  [[nodiscard]] virtual QString panelTitle() const = 0;
  [[nodiscard]] virtual QString description() const = 0;
  [[nodiscard]] virtual QUrl panelSource() const = 0;
  [[nodiscard]] virtual QObject* createBackend(QObject* parent) = 0;
};

#define HMI_PLUGIN_IID "org.senxplane.hmi.HmiPlugin/1.0"
Q_DECLARE_INTERFACE(HmiPlugin, HMI_PLUGIN_IID)
