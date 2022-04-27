#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtSql/QSqlDatabase>
#include "main_window.h"

namespace jp
{
    class Controller
    {
    public:
        Controller(MainWindow& window, QSqlDatabase& db);
        void LoadderOrder();

    private:
        MainWindow& window_;
        QSqlDatabase& db_;
    };


}

#endif // CONTROLLER_H
