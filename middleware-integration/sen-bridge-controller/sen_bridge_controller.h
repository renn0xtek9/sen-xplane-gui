#pragma once

#include <QObject>
#include <memory>

class SenBridgeController final : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
  explicit SenBridgeController(QObject* parent = nullptr);
  ~SenBridgeController() override;

  [[nodiscard]] bool connected() const;

  Q_INVOKABLE void connectToSen();

public slots:
  void stop();

signals:
  void connectedChanged();
  void connectionEstablished();

private:
  class SenBridgeControllerPrivate;
  std::unique_ptr<SenBridgeControllerPrivate> d;
};
