
#include <QApplication>
#include <QDebug>
#include "windows/authorization_window.h"
#include "controller.h"
#include "data_base.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    jp::DataBase db;

    if (db.connection())
    {
        AuthorizationWindow auth_window;
        jp::Controller controller(auth_window, db.getContext());

        QObject::connect(&auth_window, &AuthorizationWindow::authorizationTriggered,
                         &controller, &jp::Controller::authorizationWindow);

        auth_window.show();
        return a.exec();
    }

    QMessageBox::critical(0, "Ошибка", "Подключение к базе данных не удалось");
    a.quit();

    return 0;
}
