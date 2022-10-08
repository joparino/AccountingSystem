
#ifndef DATALAYER_H
#define DATALAYER_H

#include "bloom.h"
#include "entities/models.h"
#include "qsqldatabase.h"

namespace jp
{
    class DataLayer
    {
    public:
        friend class Controller;
        friend class Logic;

        typedef std::list<std::shared_ptr<Book>>      Books;
        typedef std::list<std::shared_ptr<Order>>     Orders;
        typedef std::list<std::shared_ptr<Author>>    Authors;
        typedef std::list<std::shared_ptr<Genre>>     Genres;
        typedef std::list<std::shared_ptr<Publisher>> Publishers;
        typedef std::list<std::shared_ptr<Client>>    Clients;
        typedef std::list<std::shared_ptr<Status>>    Statuses;
        typedef std::list<std::shared_ptr<Employee>>  Employees;

        DataLayer(QSqlDatabase& db);

    private:
        QSqlDatabase& db_;
        jp::BloomFilter bf_;
        jp::BloomFilter bfb_;
        Books books_;
        Orders orders_;
        Authors authors_;
        Genres genres_;
        Publishers publishers_;
        Clients clients_;
        Statuses statuses_;
        Employees employees_;
    };
}

#endif
