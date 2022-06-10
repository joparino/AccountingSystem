
#include <QApplication>
#include <QDebug>
#include "windows/authorization_window.h"
#include "windows/main_window.h"
#include "data_base.h"
#include "bloom.h"
#include "controller.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

    jp::DataBase db;
    AddClientWindow acw;
    AddBookWindow abw;
    AddArrivalWindow aw;
    AddOrderWindow aow(acw);
    OrderWindow ow;
    MainWindow w(ow, aow, abw, aw);
    AuthorizationWindow auth_window;

    db.connection();

    jp::Controller c(w, ow, acw, aow, abw, aw, db.getContext());

    QObject::connect(&auth_window, &AuthorizationWindow::authorizationTriggered,
                     &c, &jp::Controller::authorization);
    QObject::connect(&w, &MainWindow::searchOrderTriggered,
                     &c, &jp::Controller::searchOrder);
    QObject::connect(&w, &MainWindow::searchBookTriggered,
                     &c, &jp::Controller::searchBook);
    QObject::connect(&acw, &AddClientWindow::addClientTriggered,
                     &c, &jp::Controller::addClient);
    QObject::connect(&abw, &AddBookWindow::addBookTriggered,
                     &c, &jp::Controller::addBook);
    QObject::connect(&aw, &AddArrivalWindow::addArrivalTriggered,
                     &c, &jp::Controller::addArrival);
    QObject::connect(&aow, &AddOrderWindow::addOrder,
                     &c, &jp::Controller::addOrder);
    QObject::connect(&ow, &OrderWindow::hasOrderChangedTriggered,
                     &c, &jp::Controller::hasOrderChanged);

    auth_window.exec();

    return a.exec();
}
