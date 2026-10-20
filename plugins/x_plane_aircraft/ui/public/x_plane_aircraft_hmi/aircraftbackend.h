#pragma once

#include <QElapsedTimer>
#include <QObject>
#include <QTimer>

class AircraftBackend final : public QObject {
  Q_OBJECT
  Q_PROPERTY(double altitude READ altitude WRITE setAltitude NOTIFY telemetryChanged)
  Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY telemetryChanged)

public:
  explicit AircraftBackend(QObject* parent = nullptr);

  void setAltitude(const double);
  void setSpeed(const double);

  [[nodiscard]] double altitude() const;
  [[nodiscard]] double speed() const;

signals:
  void telemetryChanged();

private slots:

private:
  QTimer timer_;
  double altitude_ = 1000.0;
  double speed_ = 200.0;
};
