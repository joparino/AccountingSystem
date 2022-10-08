
#ifndef LOGIC_H
#define LOGIC_H

#include <QtSql/QSqlDatabase>
#include <QTableWidget>
#include "entities/models.h"
#include "data_layer.h"

namespace jp
{
    class Logic
    {
       friend class Controller;

    public:
        Logic(QSqlDatabase& db);

        int authorization(std::string& login, std::string& password);

        DataLayer::Orders searchOrder(std::string& str);
        DataLayer::Books searchBook(std::string& str);

        bool addBook(std::shared_ptr<Book> book);
        bool addOrder(std::string& number, std::string& address, std::string& sum, std::list<jp::Book> books);
        bool addEmployee(std::shared_ptr<jp::Employee> employee);
        bool addClient(std::string& name, std::string& number);
        bool addArrival(std::shared_ptr<Book>, std::string& count);
        bool addPublisher(std::shared_ptr<jp::Publisher> publisher);
        bool addAuthor(std::shared_ptr<jp::Author> author);
        bool addGenre(std::shared_ptr<jp::Genre> genre);


        bool changeBook(std::shared_ptr<jp::Book> book);
        bool changeOrder(std::shared_ptr<jp::Order> order, int status);
        bool changeEmployee(std::shared_ptr<jp::Employee> employee);
        bool changePublisher(std::shared_ptr<jp::Publisher> publisher);
        bool changeAuthor(std::shared_ptr<jp::Author> author);
        bool changeGenre(std::shared_ptr<jp::Genre> genre);

        std::vector<float> getSales();
        std::vector<std::pair<std::string, int>> getTopBook();
        std::vector<std::pair<std::string, int>> getTopBookMonth();
        std::vector<std::pair<std::string, int>> getTopClient();

    private:
        QSqlDatabase& db_;
        DataLayer     data_layer_;
        std::shared_ptr<jp::Employee> authorized_employee_;
    };
}

#endif
