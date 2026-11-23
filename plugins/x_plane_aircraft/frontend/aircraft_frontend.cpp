#include <QObject>
#include <QPair>
#include <QString>
#include <QTime>
#include <QVariant>
#include <QVariantMap>
#include <QtCore/qcontainerfwd.h>
#include <QtCore/qtmetamacros.h>

#include <hmi/plugin_manager_interface.h>

#include <x_plane_aircraft_hmi/aircraft_frontend.h>

AircraftFrontend::AircraftFrontend(QObject* parent, hmi::PluginManagerInterface* pluginManager)
    : QObject(parent), pluginManager_(pluginManager) {
  altitudes_.append(qMakePair(QTime::currentTime(), altitude_));
}

double AircraftFrontend::altitude() const {
  return altitude_;
}

double AircraftFrontend::speed() const {
  return speed_;
}

void AircraftFrontend::increaseAltitude() {
  emit increaseAltitudeRequested();
}

void AircraftFrontend::openGraphView() {
  if (pluginManager_ != nullptr) {
    pluginManager_->instantiateView(QStringLiteral("x_plane_aircraft"), QStringLiteral("aircraftGraph"));
  }
}

void AircraftFrontend::setAltitude(const double& altitude) {
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

void AircraftFrontend::setSpeed(const double& speed) {
  if (speed_ == speed) {
    return;
  }

  speed_ = speed;
  emit telemetryChanged();
}
