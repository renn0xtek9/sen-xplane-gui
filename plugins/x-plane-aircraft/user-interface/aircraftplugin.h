#pragma once

#include "plugininterface.h"

#include <QObject>

class AircraftPlugin final : public QObject, public HmiPlugin {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID HMI_PLUGIN_IID)
  Q_INTERFACES(HmiPlugin)

public:
  [[nodiscard]] QString id() const override;
  [[nodiscard]] QString name() const override;
  [[nodiscard]] QString panelTitle() const override;
  [[nodiscard]] QString description() const override;
  [[nodiscard]] QUrl panelSource() const override;
  [[nodiscard]] QObject *createBackend(QObject *parent) override;
};
