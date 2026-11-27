
#include <QObject>
#include <QSignalSpy>
#include <QTest>
#include <QTime>
#include <QVariantList>
#include <QVariantMap>

#include "dummy_plugin_manger.h"
#include <x_plane_aircraft_hmi/aircraft_frontend.h>

class AircraftFrontendTest : public QObject {
  Q_OBJECT
public:
  AircraftFrontendTest() : unit(nullptr, &manager_), unitWithoutManager(nullptr, nullptr) {}
  hmi::DummyPluginManager manager_;
  AircraftFrontend unit;

  AircraftFrontend unitWithoutManager;

private slots:

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void initTestCase() {
    qDebug("Called before everything else.");
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void initializationTest() const {
    QCOMPARE(unit.altitude(), 0.0);
    QCOMPARE(unit.speed(), 0.0);
    QCOMPARE(unit.altitudes().size(), 0);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void increaseAltitudeTest() {
    QSignalSpy increaseAltitudeSpy(&unit, SIGNAL(increaseAltitudeRequested()));
    unit.increaseAltitude();
    QCOMPARE(increaseAltitudeSpy.count(), 1);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void openGraphViewTest() {
    QCOMPARE(manager_.instantiateViewCallArgs().size(), 0);
    unit.openGraphView();
    QCOMPARE(manager_.instantiateViewCallArgs().size(), 1);
    QCOMPARE(manager_.instantiateViewCallArgs().front().first, "x_plane_aircraft");
    QCOMPARE(manager_.instantiateViewCallArgs().front().second, "aircraftGraph");

    // When manager is not set, the plugin shall not crash.
    unitWithoutManager.openGraphView();
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void setSpeedTest() {
    QSignalSpy telemetrySpy(&unit, SIGNAL(telemetryChanged()));

    // When setting an speed
    unit.setSpeed(123.4);
    QCOMPARE(unit.speed(), 123.4);
    QCOMPARE(telemetrySpy.count(), 1);

    // When setting the same speed again
    unit.setSpeed(123.4);
    QCOMPARE(unit.speed(), 123.4);
    QCOMPARE(telemetrySpy.count(), 1);

    // when setting a negative speed
    unit.setSpeed(-123.4);
    QCOMPARE(unit.speed(), -123.4);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void setAltitudeTest() {
    const QTime now = QTime::currentTime();
    const auto oneSecondLater = now.addSecs(1);
    const auto twoSecondLater = now.addSecs(2);
    const auto sixSecondLater = now.addSecs(6);
    const auto sample = [](const QTime& time, double altitude) {
      return QVariantMap{
          {QStringLiteral("time"), time.msecsSinceStartOfDay()},
          {QStringLiteral("altitude"), altitude},
      };
    };
    QVariantList expectedAltitudes;

    QSignalSpy telemetrySpy(&unit, SIGNAL(telemetryChanged()));
    QSignalSpy altitudeTableSpy(&unit, SIGNAL(altitudeTableChanged()));

    unit.setAltitude(2000.0, now);
    expectedAltitudes.append(sample(now, 2000.0));
    QCOMPARE(unit.altitude(), 2000.0);
    QCOMPARE(telemetrySpy.count(), 1);
    QCOMPARE(altitudeTableSpy.count(), 1);
    QCOMPARE(unit.altitudes(), expectedAltitudes);

    unit.setAltitude(2001.0, oneSecondLater);
    expectedAltitudes.append(sample(oneSecondLater, 2001.0));
    QCOMPARE(unit.altitude(), 2001.0);
    QCOMPARE(telemetrySpy.count(), 2);
    QCOMPARE(altitudeTableSpy.count(), 2);
    QCOMPARE(unit.altitudes(), expectedAltitudes);

    // when setting again the same altitude, it won't update
    unit.setAltitude(2001.0, twoSecondLater);
    QCOMPARE(unit.altitude(), 2001.0);
    QCOMPARE(telemetrySpy.count(), 2);
    QCOMPARE(altitudeTableSpy.count(), 2);
    QCOMPARE(unit.altitudes(), expectedAltitudes);

    // Samples older than five seconds are removed.
    unit.setAltitude(2002.0, sixSecondLater);
    expectedAltitudes.removeFirst();
    expectedAltitudes.removeFirst();
    expectedAltitudes.append(sample(sixSecondLater, 2002.0));
    QCOMPARE(unit.altitude(), 2002.0);
    QCOMPARE(telemetrySpy.count(), 3);
    QCOMPARE(altitudeTableSpy.count(), 3);
    QCOMPARE(unit.altitudes(), expectedAltitudes);
  }

  // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
  void cleanupTestCase() {
    qDebug("Called after myFirstTest and mySecondTest.");
  }
};

#include "aircraft_frontend_test.moc"

QTEST_APPLESS_MAIN(AircraftFrontendTest)
