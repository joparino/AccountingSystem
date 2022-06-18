
#include <QApplication>
#include <QDebug>
#include "app_business_logic.h"
#include "windows/administrator_window.h"
#include "windows/authorization_window.h"
#include "windows/main_window.h"
#include "controller.h"
#include "data_base.h"
#include "bloom.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

    jp::DataBase db;
    AuthorizationWindow auth_window;

    db.connection();

    jp::AppBusinessLogic apl(auth_window, db.getContext());

    QObject::connect(&auth_window, &AuthorizationWindow::authorizationTriggered,
                     &apl, &jp::AppBusinessLogic::authorizationWindow);


    auth_window.show();

    return a.exec();
}
