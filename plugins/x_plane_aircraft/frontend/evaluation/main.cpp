#include <QCoreApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QResource>
#include <QTimer>
#include <QtCore/QtResource>

#include <cstdlib>

#include <x_plane_aircraft_hmi/aircraft_frontend.h>

int main(int argc, char* argv[]) {
  QGuiApplication application(argc, argv);
  QGuiApplication::setApplicationName("X-Plane Aircraft UI Evaluation");

  Q_INIT_RESOURCE(aircraft_qml);

  AircraftFrontend aircraftFrontend;
  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("aircraftFrontend", &aircraftFrontend);
  engine.loadFromModule("XPlaneAircraftEvaluation", "AircraftPanelEval");
  engine.loadFromModule("XPlaneAircraftEvaluation", "AircraftGraphEval");

  if (engine.rootObjects().isEmpty()) {
    return EXIT_FAILURE;
  }

  if (QGuiApplication::arguments().contains("--smoke-test")) {
    QTimer::singleShot(0, &application, &QCoreApplication::quit);
  }

  return QGuiApplication::exec();
}
