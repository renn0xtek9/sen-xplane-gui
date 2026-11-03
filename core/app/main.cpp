#include <hmi/plugin_factory_interface.h>
#include <hmi/plugin_sen_factory.h>
#include <hmi/pluginmanager.h>
#include <hmi/sen_bridge_controller.h>

#include <QDir>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include <cstdlib>

class HmiState final : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
  using QObject::QObject;

  [[nodiscard]] bool connected() const {
    return connected_;
  }

  Q_INVOKABLE void notifySenObjectReceived() {
    if (connected_) {
      return;
    }

    connected_ = true;
    emit connectedChanged();
  }

signals:
  void connectedChanged();

private:
  bool connected_ = false;
};

int main(int argc, char* argv[]) {
  QGuiApplication application(argc, argv);
  QGuiApplication::setApplicationName("Sen X-Plane HMI");

  const QString pluginDirectory = QDir(QCoreApplication::applicationDirPath()).filePath("plugins");
  auto pluginSenFactory = std::make_shared<hmi::PluginSenFactory>();
  std::vector<std::shared_ptr<hmi::PluginFactory>> pluginFactories = {pluginSenFactory};
  hmi::PluginManager pluginManager(pluginDirectory, pluginFactories);
  const bool smokeTest = QGuiApplication::arguments().contains("--smoke-test");
  if (smokeTest && !pluginManager.hasPlugin("x_plane_aircraft")) {
    return EXIT_FAILURE;
  }
  HmiState hmiState;
  hmi::SenBridgeController senBridgeController;
  senBridgeController.addPlugins(pluginManager.getPlugins());
  QObject::connect(&senBridgeController, &hmi::SenBridgeController::connectionEstablished, &hmiState,
                   &HmiState::notifySenObjectReceived);

  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("hmiState", &hmiState);
  engine.rootContext()->setContextProperty("pluginManager", &pluginManager);
  engine.rootContext()->setContextProperty("senBridge", &senBridgeController);
  engine.loadFromModule("SenXplaneHmi", "Main");

  if (engine.rootObjects().isEmpty()) {
    return EXIT_FAILURE;
  }

  if (smokeTest) {
    QTimer::singleShot(5000, &application, [&application] { QGuiApplication::quit(); });
  }

  return QGuiApplication::exec();
}

#include "main.moc"
