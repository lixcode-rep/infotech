#include "uploader.h"
#include <iostream>

Uploader::Uploader(const QString &local, const QString &remote)
    : Tasker(local, remote) {
  connect(manager, &QNetworkAccessManager::finished, this,
          &Uploader::handleResponse);
}

void Uploader::run() {
  QString url = "https://graph.microsoft.com/v1.0/me/drive/root:/%1:/content";
  QNetworkRequest request(url.arg(remote));

  QFile file(local);
  QDataStream stream(&file);

  if (!file.open(QFile::ReadOnly)) {
    std::cout << "Не получилось открыть файл для чтения." << std::endl;
    emit requestStop();
    return;
  }

  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/octet-stream");
  request.setRawHeader(QByteArray("Authorization"),
                       ("bearer " + code).toLatin1());

  manager->put(request, file.readAll());
}

void Uploader::handleResponse(QNetworkReply *reply) {
  if (reply->error() == QNetworkReply::NoError &&
      (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() ==
           200 ||
       reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() ==
           201)) {
    std::cout << "Файл загружен успешно!" << std::endl;
  } else {
    std::cout << "Ошибка: " << reply->errorString().toStdString() << std::endl;
  }
  qDebug() << reply->error() << reply->errorString()
           << QString(reply->readAll())
           << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  emit requestStop();
}
