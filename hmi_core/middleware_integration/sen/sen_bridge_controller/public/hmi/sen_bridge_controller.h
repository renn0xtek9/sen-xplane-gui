#pragma once
#include <QObject>

#include <hmi/plugin_interface.h>
#include <hmi/plugin_sen.h>

#include <sen/kernel/component.h>

#include <memory>

class SenBridgeController final : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

public:
  explicit SenBridgeController(QObject* parent = nullptr);
  ~SenBridgeController() override;

  [[nodiscard]] bool connected() const;

  Q_INVOKABLE void connectToSen();
  void addPlugins(std::vector<std::shared_ptr<HmiPlugin>> plugins);

  // Implement this to add component to load.
  //  void addComponentToLoad(std::shared_ptr<sen::kernel::Component>, const std::string& name);

public slots:
  void stop();

signals:
  void connectedChanged();
  void connectionEstablished();

private:
  class SenBridgeControllerPrivate;
  std::unique_ptr<SenBridgeControllerPrivate> d;
  std::vector<std::shared_ptr<HmiPluginSen>> m_plugins{};
};
