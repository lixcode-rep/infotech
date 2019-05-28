#include "downloader.h"

#include <iostream>

Downloader::Downloader(const QString &remote, const QString &local)
    : Tasker(local, remote) {
  connect(manager, &QNetworkAccessManager::finished, this,
          &Downloader::handleResponse);
}

void Downloader::run() {
  QString url = "https://graph.microsoft.com/v1.0/me/drive/root:/%1:/content";
  QNetworkRequest request(url.arg(remote));

  request.setRawHeader(QByteArray("Authorization"),
                       ("bearer " + code).toLatin1());

  manager->get(request);
}

void Downloader::handleResponse(QNetworkReply *reply) {
  if (reply->error() == QNetworkReply::NoError &&
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() ==
          302) {
    QString to = reply->attribute(QNetworkRequest::RedirectionTargetAttribute)
                     .toString();
    QNetworkRequest request(to);
    manager->get(request);
    qDebug() << "redirected to" << to;

  } else if (reply->error() == QNetworkReply::NoError &&
             reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                     .toInt() == 200) {
    std::cout << "Файл загружен успешно!" << std::endl;

    QFile file(local);
    QDataStream stream(&file);

    if (!file.open(QFile::WriteOnly)) {
      std::cout << "Не получилось открыт файл для записи." << std::endl;
    } else {
      QByteArray data = reply->readAll();
      stream.writeRawData(data.data(), data.length());
    }
    emit requestStop();
  } else {
    std::cout << "Ошибка: " << reply->errorString().toStdString() << std::endl;
    emit requestStop();
  }
  qDebug() << reply->error() << reply->errorString()
           << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
}
