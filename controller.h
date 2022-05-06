#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtSql/QSqlDatabase>
#include "bloom.h"
#include "main_window.h"

namespace jp
{
    class Controller : public QObject
    {
       Q_OBJECT
    public:
        Controller(MainWindow& window, QSqlDatabase& db);
        void loader() noexcept;

    public slots:
        void searchOrder(QString str);

    private:
        MainWindow& window_;
        QSqlDatabase& db_;
        jp::BloomFilter bf_;
    };


}

#endif // CONTROLLER_H
