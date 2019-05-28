#ifndef TASKER_H
#define TASKER_H

#include <QString>
#include <o2requestor.h>
#include <o2skydrive.h>

class Tasker : public QObject {
  Q_OBJECT

public:
  Tasker(const QString &local, const QString &remote);

  void setCode(const QString &code);
  virtual void run() = 0;

signals:
  void requestStop();

protected:
  // upload file data
  QString local;
  QString remote;

  // Network objects
  QNetworkAccessManager *manager;

  // Auth code
  QString code;
};

#endif // TASKER_H
