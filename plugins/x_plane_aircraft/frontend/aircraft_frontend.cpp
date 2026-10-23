#include <x_plane_aircraft_hmi/aircraft_frontend.h>

#include <QVariantMap>

AircraftFrontend::AircraftFrontend(QObject* parent) : QObject(parent) {
  altitudes_.append(qMakePair(QTime::currentTime(), altitude_));
}

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

  const QTime now = QTime::currentTime();
  altitudes_.append(qMakePair(now, altitude));

  while (!altitudes_.isEmpty()) {
    int age = altitudes_.first().first.msecsTo(now);
    if (age < 0) {
      age += 24 * 60 * 60 * 1000;
    }
    if (age <= 5000) {
      break;
    }
    altitudes_.removeFirst();
  }

  altitude_ = altitude;
  emit telemetryChanged();
  emit altitudeTableChanged();
}

QVariantList AircraftFrontend::altitudes() const {
  QVariantList samples;
  samples.reserve(altitudes_.size());
  for (const auto& [time, altitude] : altitudes_) {
    samples.append(
        QVariantMap{{QStringLiteral("time"), time.msecsSinceStartOfDay()}, {QStringLiteral("altitude"), altitude}});
  }
  return samples;
}

void AircraftFrontend::setSpeed(const double speed) {
  if (speed_ == speed) {
    return;
  }

  speed_ = speed;
  emit telemetryChanged();
}
