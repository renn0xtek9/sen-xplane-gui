#include <hmi/plugin_sen_factory.h>
#include <hmi/pluginmanager.h>
#include <hmi/sen_bridge_controller.h>

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDockWidget>
#include <QHash>
#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QQmlError>
#include <QQuickItem>
#include <QQuickWidget>
#include <QSizePolicy>
#include <QStatusBar>
#include <QTimer>
#include <QVariantMap>

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

class FloatingMainWindow final : public QMainWindow {
public:
  FloatingMainWindow(PluginManager& pluginManager, HmiState& hmiState, SenBridgeController& senBridgeController)
      : pluginManager_(pluginManager) {
    setWindowTitle(QStringLiteral("Sen X-Plane HMI"));
    resize(1280, 800);
    setDockNestingEnabled(true);

    auto* placeholder =
        new QLabel(QStringLiteral("Drag a panel tab to detach it, or drag a floating panel here to dock."));
    placeholder->setAlignment(Qt::AlignCenter);
    setCentralWidget(placeholder);

    auto* fileMenu = menuBar()->addMenu(QStringLiteral("&File"));
    auto* connectAction = fileMenu->addAction(QStringLiteral("Connect to Sen"));
    QObject::connect(connectAction, &QAction::triggered, &senBridgeController, &SenBridgeController::connectToSen);
    fileMenu->addSeparator();
    auto* exitAction = fileMenu->addAction(QStringLiteral("Exit"));
    QObject::connect(exitAction, &QAction::triggered, this, &QWidget::close);

    viewMenu_ = menuBar()->addMenu(QStringLiteral("&View"));
    pluginMenu_ = menuBar()->addMenu(QStringLiteral("&Plugin"));

    statusLabel_ = new QLabel(QStringLiteral("Not connected"));
    statusBar()->addPermanentWidget(statusLabel_);
    QObject::connect(&hmiState, &HmiState::connectedChanged, statusLabel_, [&hmiState, this] {
      statusLabel_->setText(hmiState.connected() ? QStringLiteral("Connected") : QStringLiteral("Not connected"));
    });

    createPanels();
    createPluginActions();
  }

  [[nodiscard]] qsizetype panelCount() const {
    return panelCount_;
  }

  [[nodiscard]] bool qmlLoadFailed() const {
    return qmlLoadFailed_;
  }

  [[nodiscard]] bool floatingPanelCanMoveAndResize() {
    if (docksByPlugin_.isEmpty() || docksByPlugin_.cbegin().value().isEmpty()) {
      return false;
    }

    QDockWidget* dock = docksByPlugin_.cbegin().value().first();
    dock->setFloating(true);
    dock->show();
    QApplication::processEvents();

    const Qt::WindowFlags flags = dock->windowFlags();
    const bool hasWindowFrame = dock->isWindow() && !flags.testFlag(Qt::FramelessWindowHint) &&
                                !flags.testFlag(Qt::MSWindowsFixedSizeDialogHint);
    const QSize minimumSize = dock->minimumSize();
    const QSize maximumSize = dock->maximumSize();

    const QSize requestedSize(640, 480);
    dock->resize(requestedSize);
    QApplication::processEvents();
    const bool canResize = dock->size() == requestedSize;

    dock->setFloating(false);
    return hasWindowFrame && minimumSize.width() < maximumSize.width() && minimumSize.height() < maximumSize.height() &&
           canResize;
  }

private:
  void createPanels() {
    QDockWidget* firstDock = nullptr;
    const QVariantList panels = pluginManager_.panels();
    for (const QVariant& panelEntry : panels) {
      const QVariantMap panel = panelEntry.toMap();
      const QString pluginId = panel.value(QStringLiteral("pluginId")).toString();
      const QString panelId = panel.value(QStringLiteral("panelId")).toString();
      const QString panelTitle = panel.value(QStringLiteral("panelTitle")).toString();
      const QUrl source = panel.value(QStringLiteral("panelSource")).toUrl();

      auto* quickWidget = new QQuickWidget;
      quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
      quickWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
      quickWidget->setMinimumSize(0, 0);
      QObject::connect(
          quickWidget, &QQuickWidget::statusChanged, quickWidget,
          [this, quickWidget, panelTitle,
           backend = panel.value(QStringLiteral("backend")).value<QObject*>()](QQuickWidget::Status status) {
            if (status == QQuickWidget::Ready && quickWidget->rootObject() != nullptr) {
              quickWidget->rootObject()->setProperty("backend", QVariant::fromValue(backend));
            } else if (status == QQuickWidget::Error) {
              qmlLoadFailed_ = true;
              for (const QQmlError& error : quickWidget->errors()) {
                qCritical().noquote() << "Could not load panel" << panelTitle << ":" << error.toString();
              }
            }
          });
      quickWidget->setSource(source);

      auto* dock = new QDockWidget(panelTitle, this);
      dock->setObjectName(QStringLiteral("%1.%2").arg(pluginId, panelId));
      dock->setAllowedAreas(Qt::AllDockWidgetAreas);
      dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
                        QDockWidget::DockWidgetFloatable);
      dock->setMinimumSize(180, 140);
      dock->setWidget(quickWidget);
      addDockWidget(Qt::LeftDockWidgetArea, dock);
      if (firstDock != nullptr) {
        splitDockWidget(firstDock, dock, Qt::Horizontal);
      } else {
        firstDock = dock;
      }

      viewMenu_->addAction(dock->toggleViewAction());
      docksByPlugin_[pluginId].append(dock);
      ++panelCount_;

      if (quickWidget->status() == QQuickWidget::Error) {
        qmlLoadFailed_ = true;
      } else if (quickWidget->status() == QQuickWidget::Ready && quickWidget->rootObject() != nullptr) {
        quickWidget->rootObject()->setProperty("backend", panel.value(QStringLiteral("backend")));
      }
    }
  }

  void createPluginActions() {
    for (const QVariant& pluginEntry : pluginManager_.plugins()) {
      const QVariantMap plugin = pluginEntry.toMap();
      const QString pluginId = plugin.value(QStringLiteral("pluginId")).toString();
      auto* action = pluginMenu_->addAction(plugin.value(QStringLiteral("pluginName")).toString());
      action->setCheckable(true);
      action->setChecked(plugin.value(QStringLiteral("pluginActive")).toBool());
      QObject::connect(action, &QAction::toggled, this, [this, pluginId](bool active) {
        pluginManager_.setPluginActive(pluginId, active);
        for (QDockWidget* dock : docksByPlugin_.value(pluginId)) {
          dock->setVisible(active);
        }
      });
    }
  }

  PluginManager& pluginManager_;
  QMenu* viewMenu_ = nullptr;
  QMenu* pluginMenu_ = nullptr;
  QLabel* statusLabel_ = nullptr;
  QHash<QString, QList<QDockWidget*>> docksByPlugin_;
  qsizetype panelCount_ = 0;
  bool qmlLoadFailed_ = false;
};

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  QApplication::setApplicationName(QStringLiteral("Sen X-Plane HMI"));

  const QString pluginDirectory = QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("plugins"));
  PluginManager pluginManager(pluginDirectory, {std::make_shared<HmiPluginSenFactory>()});
  const bool smokeTest = application.arguments().contains(QStringLiteral("--smoke-test"));
  if (smokeTest && !pluginManager.hasPlugin(QStringLiteral("x_plane_aircraft"))) {
    qCritical() << "Smoke test could not find the X-Plane aircraft plugin.";
    return EXIT_FAILURE;
  }

  HmiState hmiState;
  SenBridgeController senBridgeController;
  senBridgeController.addPlugins(pluginManager.getPlugins());
  QObject::connect(&senBridgeController, &SenBridgeController::connectionEstablished, &hmiState,
                   &HmiState::notifySenObjectReceived);

  FloatingMainWindow mainWindow(pluginManager, hmiState, senBridgeController);
  if (smokeTest && (mainWindow.panelCount() == 0 || mainWindow.qmlLoadFailed())) {
    return EXIT_FAILURE;
  }
  mainWindow.show();

  if (smokeTest) {
    if (!mainWindow.floatingPanelCanMoveAndResize()) {
      qCritical() << "Floating panels must have a movable window frame and support resizing.";
      return EXIT_FAILURE;
    }
    QTimer::singleShot(1000, &application, [&application, &mainWindow] {
      application.exit(mainWindow.qmlLoadFailed() ? EXIT_FAILURE : EXIT_SUCCESS);
    });
  }

  return application.exec();
}

#include "main.moc"