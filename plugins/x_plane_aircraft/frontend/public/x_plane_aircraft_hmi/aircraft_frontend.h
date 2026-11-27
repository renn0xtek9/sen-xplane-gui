#pragma once

#include <QElapsedTimer>
#include <QObject>
#include <QPair>
#include <QTime>
#include <QTimer>
#include <QVariantList>
#include <QVector>

namespace hmi {
class PluginManagerInterface;
}

class AircraftFrontend final : public QObject {
  Q_OBJECT
  Q_PROPERTY(double altitude READ altitude WRITE setAltitude NOTIFY telemetryChanged)
  Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY telemetryChanged)
  Q_PROPERTY(QVariantList altitudeHistory READ altitudes NOTIFY altitudeTableChanged)

public:
  explicit AircraftFrontend(QObject* parent = nullptr, hmi::PluginManagerInterface* pluginManager = nullptr);

  void setAltitude(const double&, QTime timeStamp = QTime::currentTime());
  void setSpeed(const double&);

  Q_INVOKABLE void increaseAltitude();
  Q_INVOKABLE void openGraphView();

  [[nodiscard]] double altitude() const;
  [[nodiscard]] double speed() const;

  Q_INVOKABLE [[nodiscard]] QVariantList altitudes() const;

signals:
  void telemetryChanged();
  void altitudeTableChanged();
  void increaseAltitudeRequested();

private:
  hmi::PluginManagerInterface* pluginManager_;
  QTimer timer_;
  double altitude_{};
  double speed_{};
  QVector<QPair<QTime, double>> altitudes_;
};
