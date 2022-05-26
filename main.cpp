
#include <QApplication>
#include <QDebug>
#include "windows/main_window.h"
#include "data_base.h"
#include "bloom.h"
#include "controller.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    jp::DataBase db;
    AddOrderWindow ow;
    AddBookWindow bw;
    AddArrivalWindow aw;
    MainWindow w(ow, bw, aw);

    db.connection();

    jp::Controller c(w, ow, bw, aw, db.getContext());

    QObject::connect(&w, &MainWindow::searchOrderTriggered,
                     &c, &jp::Controller::searchOrder);
    QObject::connect(&w, &MainWindow::searchBookTriggered,
                     &c, &jp::Controller::searchBook);
    QObject::connect(&bw, &AddBookWindow::addBookTriggered,
                     &c, &jp::Controller::addBook);
    QObject::connect(&aw, &AddArrivalWindow::addArrivalTriggered,
                     &c, &jp::Controller::addArrival);

    c.loaderOrder();
    c.loaderBook();
    c.loaderOrderWindow();
    c.loaderBookWindow();

    w.show();

    return a.exec();
}
