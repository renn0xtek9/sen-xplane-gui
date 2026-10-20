#include <x_plane_aircraft_hmi/aircraftbackend.h>

#include <cmath>
#include <numbers>

AircraftBackend::AircraftBackend(QObject* parent) : QObject(parent) {
}

double AircraftBackend::altitude() const {
  return altitude_;
}

double AircraftBackend::speed() const {
  return speed_;
}

void AircraftBackend::setAltitude(const double altitude){
  altitude_=altitude;
}
void AircraftBackend::setSpeed(const double speed){
  speed_=speed;
}
