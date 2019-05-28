#include <QApplication>
#include <iostream>
#include <onedrive.h>

void myMessageOutput(QtMsgType, const QMessageLogContext &, const QString &) {}

int main(int argc, char **argv) {

  if (argc != 6) {
    std::cout << "Пропущены параметры командной строки." << std::endl;
    std::cout << "Используете следующий синтаксис: " << *argv
              << " login password put/get "
                 "src_path dst_path"
              << std::endl;
    return 0;
  }

  QApplication app(argc, argv);

  app.setApplicationName("infotech");
  app.setOrganizationDomain("tech.info");
  app.setOrganizationName("test");

  qInstallMessageHandler(myMessageOutput);

  OneDrive service(app.arguments()[1], app.arguments()[2]);

  QString operation = app.arguments()[3];

  if (operation == "put") {
    service.upload(app.arguments()[4], app.arguments()[5]);
  } else if (operation == "get") {
    service.download(app.arguments()[4], app.arguments()[5]);
  } else {
    std::cout << "Неизвестная команда: " << operation.toStdString()
              << std::endl;
    return 0;
  }

  // Link to microsoft account and release the task
  service.link();

  return app.exec();
}
