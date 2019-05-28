#ifndef UPLOADER_H
#define UPLOADER_H

#include "tasker.h"

class Uploader : public Tasker {
  Q_OBJECT

public:
  Uploader(const QString &local, const QString &remote);

  void run() override;

public slots:
  void handleResponse(QNetworkReply *reply);
};

#endif // UPLOADER_H
