#pragma once

#include <QElapsedTimer>
#include <QObject>
#include <QTimer>

class AircraftBackend final : public QObject {
  Q_OBJECT
  Q_PROPERTY(double altitude READ altitude NOTIFY telemetryChanged)
  Q_PROPERTY(double speed READ speed NOTIFY telemetryChanged)

public:
  explicit AircraftBackend(QObject* parent = nullptr);

  [[nodiscard]] double altitude() const;
  [[nodiscard]] double speed() const;

signals:
  void telemetryChanged();

private slots:
  void updateTelemetry();

private:
  QElapsedTimer elapsed_;
  QTimer timer_;
  double altitude_ = 1000.0;
  double speed_ = 200.0;
};
