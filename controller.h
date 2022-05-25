
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtSql/QSqlDatabase>
#include "bloom.h"
#include "windows/main_window.h"

namespace jp
{
    class Controller : public QObject
    {
       Q_OBJECT

    public:
        Controller(MainWindow& window, AddOrderWindow& ow, AddBookWindow& bw, QSqlDatabase& db);
        void loader() noexcept;

    public slots:
        void searchOrder(QString str);
        void searchBook(QString str);
        void addBook(QString title, QString author, QString genre,
                     QString publisher, QString year, QString price);

    private:
        MainWindow& window_;
        AddOrderWindow& ow_;
        AddBookWindow& bw_;
        QSqlDatabase& db_;
        jp::BloomFilter bf_;
        jp::BloomFilter bfb_;
    };


}

#endif
