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
    : QObject(parent), pluginManager_(pluginManager) {}

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

void AircraftFrontend::setAltitude(const double& altitude, const QTime timeStamp) {
  if (altitude_ == altitude) {
    return;
  }

  altitudes_.append(qMakePair(timeStamp, altitude));

  while (!altitudes_.isEmpty()) {
    int ageOfFirst = altitudes_.first().first.msecsTo(timeStamp);
    if (ageOfFirst >= 5000) {
      altitudes_.removeFirst();
    } else {
      break;
    }
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
