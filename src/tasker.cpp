#include "tasker.h"

Tasker::Tasker(const QString &local, const QString &remote)
    : local(local), remote(remote) {
  manager = new QNetworkAccessManager(nullptr);
}

void Tasker::setCode(const QString &code) { this->code = code; }
