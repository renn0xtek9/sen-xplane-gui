#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

class HmiState final : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
  using QObject::QObject;

  [[nodiscard]] bool connected() const { return connected_; }

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

int main(int argc, char *argv[]) {
  QGuiApplication application(argc, argv);
  QGuiApplication::setApplicationName("Sen X-Plane HMI");

  HmiState hmiState;
  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("hmiState", &hmiState);
  engine.loadFromModule("SenXplaneHmi", "Main");

  if (engine.rootObjects().isEmpty()) {
    return EXIT_FAILURE;
  }

  if (application.arguments().contains("--smoke-test")) {
    QTimer::singleShot(5000, &application,
                       [&application] { application.quit(); });
  }

  return application.exec();
}

#include "main.moc"
