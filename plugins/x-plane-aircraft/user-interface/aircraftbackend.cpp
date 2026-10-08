#include "aircraftbackend.h"

#include <cmath>
#include <numbers>

AircraftBackend::AircraftBackend(QObject* parent) : QObject(parent) {
  elapsed_.start();
  connect(&timer_, &QTimer::timeout, this, &AircraftBackend::updateTelemetry);
  updateTelemetry();
  timer_.start(50);
}

double AircraftBackend::altitude() const {
  return altitude_;
}

double AircraftBackend::speed() const {
  return speed_;
}

void AircraftBackend::updateTelemetry() {
  const double elapsedMilliseconds = static_cast<double>(elapsed_.elapsed());
  const double phase = 2.0 * std::numbers::pi * elapsedMilliseconds;
  altitude_ = 1000.0 + 10.0 * std::sin(phase / 10000.0);
  speed_ = 200.0 + 10.0 * std::sin(phase / 5000.0);
  emit telemetryChanged();
}
