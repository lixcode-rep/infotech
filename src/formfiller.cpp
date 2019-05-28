#include "formfiller.h"
#include <iostream>

FormFiller::FormFiller(QWebEngineView *web, const QString &user,
                       const QString &password)
    : web(web), user(user), password(password) {}

void FormFiller::run() {
  // Accept the application rights
  web->page()->runJavaScript(
      R"(try{document.getElementById("idBtn_Accept").click()}catch(e){})");

  // Submit username
  web->page()->runJavaScript(
      "try{document.querySelector('input[type=email]').value = '" + user +
      "'}catch(e){}");

  // Submit password
  web->page()->runJavaScript(
      "try{document.querySelector('input[type=password]').value = '" +
      password + "'}catch(e){}");

  // Check if there are errors
  web->page()->runJavaScript(
      R"(document.querySelector("input.has-error") != null)",
      [this](const QVariant &res) {
        // The form are submited successfully after the third try
        if (res.type() == QVariant::Bool && res.toBool() && errors++ >= 2) {
          std::cout << "Неправильный логин или пароль." << std::endl;
          emit requestClose();
        } else {
          web->page()->runJavaScript("try{document.querySelector('form[name=f1]"
                                     "').submit()}catch(e){}");
        }
      });
}
