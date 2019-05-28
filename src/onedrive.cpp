#include "onedrive.h"
#include "downloader.h"
#include "formfiller.h"
#include <QApplication>
#include <QString>
#include <QThread>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <iostream>
#include <qwebengineview.h>

OneDrive::OneDrive(const QString &user, const QString &password)
    : user(user), password(password) {

  o2settingsStore = new O0SettingsStore("qweqazcxzde");

  onedrive.setStore(o2settingsStore);
  onedrive.setClientId(clientId);
  onedrive.setClientSecret(clientSecret);

  connect(&onedrive, &O2Skydrive::linkingFailed, this,
          &OneDrive::onLinkingFailed, Qt::QueuedConnection);
  connect(&onedrive, &O2Skydrive::linkingSucceeded, this,
          &OneDrive::onLinkingSucceeded, Qt::QueuedConnection);
  connect(&onedrive, &O2Skydrive::openBrowser, this, &OneDrive::onOpenBrowser,
          Qt::QueuedConnection);
  connect(&onedrive, &O2Skydrive::closeBrowser, this, &OneDrive::onCloseBrowser,
          Qt::QueuedConnection);
  connect(&onedrive, &O2Skydrive::tokenChanged, this,
          &OneDrive::onTokenChanged);

  onedrive.setScope("files.readwrite+offline_access");
}

void OneDrive::link() {
  std::cout << "Процесс аутентификации запущен." << std::endl;
  onedrive.link();
}

void OneDrive::unlink() { onedrive.unlink(); }

void OneDrive::upload(const QString &local, const QString &remote) {
  uploader = new Uploader(local, remote);

  connect(uploader, &Uploader::requestStop, this, &OneDrive::onRequestClose);

  qDebug() << "upload";
  if (onedrive.linked()) {
    //    uploader->run();
    onedrive.unlink();
    onedrive.link();
  }
}

void OneDrive::download(const QString &remote, const QString &local) {
  downloader = new Downloader(remote, local);

  connect(downloader, &Downloader::requestStop, this,
          &OneDrive::onRequestClose);

  if (onedrive.linked()) {
    onedrive.unlink();
    onedrive.link();
  }
}

void OneDrive::onLinkingFailed() {
  qDebug() << "failed";
  onRequestClose();
}

void OneDrive::onLinkingSucceeded() { qDebug() << "linked"; }

void OneDrive::onOpenBrowser(QUrl url) {
  o2authWindow = new QWebEngineView(nullptr);
  o2authWindow->page()->profile()->setPersistentCookiesPolicy(
      QWebEngineProfile::NoPersistentCookies);
  //  o2authWindow->show();
  o2authWindow->load(url);
  o2authWindow->resize(600, 600);

  formFiller = new FormFiller(o2authWindow, user, password);

  connect(formFiller, &FormFiller::requestClose, this,
          &OneDrive::onRequestClose);

  connect(o2authWindow, &QWebEngineView::loadFinished, [this](bool success) {
    qDebug() << o2authWindow->url();

    if (!success) {
      std::cout << "Проблемы с подключением к сети." << std::endl;
      return;
    }

    QString url = o2authWindow->url().toString();
    if (url.indexOf("https://login.live.com/oauth20_desktop.srf?code") == 0) {
      onedrive.redirected(o2authWindow->url());
      return;
    }

    formFiller->start();
  });

  qDebug() << url;
}

void OneDrive::onCloseBrowser() {
  o2authWindow->hide();
  //  o2authWindow->deleteLater();
}

void OneDrive::onTokenChanged() {
  if (!onedrive.token().isEmpty()) {
    std::cout << "Процесс аутентификации завершен." << std::endl;
    if (uploader != nullptr) {
      uploader->setCode(onedrive.token());
      uploader->run();
    }
    if (downloader != nullptr) {
      downloader->setCode(onedrive.token());
      downloader->run();
    }
  }
}

void OneDrive::onRequestClose() {
  onedrive.unlink();

  //  if (o2authWindow != nullptr)
  //    o2authWindow->close();

  qApp->quit();
}
