#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QObject>
#include <QPluginLoader>
#include <QVector>
#include <QtCore/qcontainerfwd.h>
#include <QtLogging>

#include <utility>

#include <hmi/plugin_instance_provider.h>
namespace hmi {

PluginInstanceProvider::PluginInstanceProvider(QString pluginDirectory)
    : plugin_directory_{std::move(pluginDirectory)} {}

QVector<QObject*> PluginInstanceProvider::getPluginInstances() {

  QVector<QObject*> result{};
  const QDir directory(plugin_directory_);
  const QFileInfoList files = directory.entryInfoList(QDir::Files);

  for (const QFileInfo& fileinfo : files) {
    if (!QLibrary::isLibrary(fileinfo.fileName())) {
      continue;
    }
    qInfo() << " Processing plugin file : " << fileinfo.absoluteFilePath();

    auto* loader = new QPluginLoader(fileinfo.absoluteFilePath(), nullptr);
    if (loader->instance() == nullptr) {
      qWarning() << fileinfo.absoluteFilePath() << " cannot be instantiated";
      loader->unload();
      delete loader;
      continue;
    }
    result.push_back(loader->instance());
    loaders_.push_back(loader);
  }
  return result;
}

} // namespace hmi
