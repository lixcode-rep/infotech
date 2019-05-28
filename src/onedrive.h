#ifndef ONEDRIVE_H
#define ONEDRIVE_H

#include "downloader.h"
#include "formfiller.h"
#include "uploader.h"

#include <QString>
#include <QWebEngineView>
#include <o0settingsstore.h>
#include <o2skydrive.h>

class OneDrive : public QObject {
  Q_OBJECT

public:
  OneDrive(const QString &user, const QString &password);

  // log in/out
  void link();
  void unlink();

  // file exchange functions
  void upload(const QString &local, const QString &remote);
  void download(const QString &remote, const QString &local);

public slots:
  // handlers for o2auth service
  void onLinkingFailed();
  void onLinkingSucceeded();
  void onOpenBrowser(QUrl url);
  void onCloseBrowser();
  void onTokenChanged();

  // function used from composition classes to close the application
  void onRequestClose();

private:
  // data from command propt console
  QString user;
  QString password;

  // application data
  O2Skydrive onedrive;
  const QString clientId = "f0fa807f-5111-427b-8457-2b99e9f05ea2";
  const QString clientSecret = "0/ukydxjxlas]A--KNOjSL43adF8T50A";

  // network variables
  FormFiller *formFiller = nullptr;
  QWebEngineView *o2authWindow = nullptr;
  O0SettingsStore *o2settingsStore = nullptr;

  // task data
  Uploader *uploader = nullptr;
  Downloader *downloader = nullptr;
};

#endif // ONEDRIVE_H
