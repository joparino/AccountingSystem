
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
        Controller(MainWindow& window, AddOrderWindow& ow, AddBookWindow& bw, AddArrivalWindow& aw, QSqlDatabase& db);
        void loaderOrder() noexcept;
        void loaderBook() noexcept;
        void loaderOrderWindow() noexcept;
        void loaderBookWindow() noexcept;

    public slots:
        void searchOrder(QString str);
        void searchBook(QString str);
        void addBook(QString title, QString author, QString genre,
                     QString publisher, QString year, QString price);
        void addArrival(QString title, QString count);

    private:
        MainWindow& window_;
        AddOrderWindow& ow_;
        AddBookWindow& bw_;
        AddArrivalWindow& aw_;
        QSqlDatabase& db_;
        jp::BloomFilter bf_;
        jp::BloomFilter bfb_;
    };


}

#endif
