
#include <QApplication>
#include <QDebug>
#include "windows/add_book_window.h"
#include "windows/add_order_window.h"
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
    MainWindow w(ow, bw);

    db.connection();

    jp::Controller c(w, ow, bw, db.getContext());

    QObject::connect(&w, &MainWindow::searchOrderTriggered,
                     &c, &jp::Controller::searchOrder);
    QObject::connect(&w, &MainWindow::searchBookTriggered,
                     &c, &jp::Controller::searchBook);
    QObject::connect(&bw, &AddBookWindow::addBookTriggered,
                     &c, &jp::Controller::addBook);

    c.loader();

    w.show();

    return a.exec();
}
