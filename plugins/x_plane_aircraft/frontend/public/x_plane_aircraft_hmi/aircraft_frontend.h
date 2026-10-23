#pragma once

#include <QElapsedTimer>
#include <QObject>
#include <QPair>
#include <QTime>
#include <QTimer>
#include <QVariantList>
#include <QVector>

class AircraftFrontend final : public QObject {
  Q_OBJECT
  Q_PROPERTY(double altitude READ altitude WRITE setAltitude NOTIFY telemetryChanged)
  Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY telemetryChanged)
  Q_PROPERTY(QVariantList altitudeHistory READ altitudes NOTIFY altitudeTableChanged)

public:
  explicit AircraftFrontend(QObject* parent = nullptr);

  void setAltitude(const double);
  void setSpeed(const double);

  [[nodiscard]] double altitude() const;
  [[nodiscard]] double speed() const;

  Q_INVOKABLE [[nodiscard]] QVariantList altitudes() const;

signals:
  void telemetryChanged();
  void altitudeTableChanged();

private slots:

private:
  QTimer timer_;
  double altitude_ = 1000.0;
  double speed_ = 200.0;
  QVector<QPair<QTime, double>> altitudes_{};
};
