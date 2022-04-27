
#include <QApplication>
#include "main_window.h"
#include "data_base.h"
#include "controller.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    jp::DataBase db;
    db.connection();

    MainWindow w;

    jp::Controller c(w, db.getContext());

    c.LoadderOrder();

    w.show();

    return a.exec();
}
