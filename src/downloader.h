#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "tasker.h"

class Downloader : public Tasker {
  Q_OBJECT

public:
  Downloader(const QString &remote, const QString &local);

  void run() override;

public slots:
  void handleResponse(QNetworkReply *reply);
};

#endif // DOWNLOADER_H
