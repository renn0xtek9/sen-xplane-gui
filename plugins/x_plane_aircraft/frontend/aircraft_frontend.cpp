#include <x_plane_aircraft_hmi/aircraft_frontend.h>

AircraftFrontend::AircraftFrontend(QObject* parent) : QObject(parent) {}

double AircraftFrontend::altitude() const {
  return altitude_;
}

double AircraftFrontend::speed() const {
  return speed_;
}

void AircraftFrontend::setAltitude(const double altitude) {
  if (altitude_ == altitude) {
    return;
  }

  altitude_ = altitude;
  emit telemetryChanged();
}

void AircraftFrontend::setSpeed(const double speed) {
  if (speed_ == speed) {
    return;
  }

  speed_ = speed;
  emit telemetryChanged();
}
