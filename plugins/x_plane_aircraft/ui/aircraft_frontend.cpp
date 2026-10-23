#include <x_plane_aircraft_hmi/aircraft_frontend.h>

AircraftFrontend::AircraftFrontend(QObject* parent) : QObject(parent) {}

double AircraftFrontend::altitude() const {
  return altitude_;
}

double AircraftFrontend::speed() const {
  return speed_;
}

void AircraftFrontend::setAltitude(const double altitude) {
  altitude_ = altitude;
}
void AircraftFrontend::setSpeed(const double speed) {
  speed_ = speed;
}
