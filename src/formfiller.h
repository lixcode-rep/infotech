#ifndef FORMFILLER_H
#define FORMFILLER_H

#include <QThread>
#include <QWebEngineView>
#include <o2skydrive.h>

class FormFiller : public QThread {
  Q_OBJECT

public:
  FormFiller(QWebEngineView *web, const QString &user, const QString &password);

signals:
  void requestClose();

protected:
  void run() override;

private:
  QWebEngineView *web;
  QString user;
  QString password;
  int errors = 0;
};

#endif // FORMFILLER_H
