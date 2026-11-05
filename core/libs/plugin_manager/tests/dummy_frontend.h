#pragma once
#include <QObject>

class DummyFrontend : public QObject {
  Q_OBJECT
public:
  explicit DummyFrontend(QObject* parent = nullptr) {}
};
