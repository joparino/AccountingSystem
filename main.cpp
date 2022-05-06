
#include <QApplication>
#include <QDebug>
#include "windows.h"
#include "main_window.h"
#include "data_base.h"
#include "bloom.h"
#include "controller.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    jp::DataBase db;
    MainWindow w;

    db.connection();

    jp::Controller c(w, db.getContext());

    QObject::connect(&w, &MainWindow::searchTriggered,
                     &c, &jp::Controller::searchOrder);

    c.loader();

    w.show();

    return a.exec();
}
