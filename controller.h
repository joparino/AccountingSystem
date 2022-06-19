
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtSql/QSqlDatabase>
#include "entities/models.h"
#include "data_layer.h"
#include "windows/administrator_window.h"
#include "windows/main_window.h"

namespace jp
{
    class Controller
    {
       friend class AppBusinessLogic;

    public:
        Controller(QSqlDatabase& db);

        int authorization(std::string login, std::string password);

        DataLayer::Orders searchOrder(QString str);
        DataLayer::Books searchBook(QString str);

        bool addBook(std::shared_ptr<Book> book);
        bool addOrder(QString number, QString address, QString sum, QTableWidget* tableBook);
        bool addEmployee(std::shared_ptr<jp::Employee> employee);
        bool addClient(std::string name, std::string number);
        bool addArrival(std::shared_ptr<Book>, QString count);
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
        bool reportSales(std::string& path);
        bool reportSellingBooks(std::string& path);
        bool reportSellingBooksMonth(std::string& path);
        bool reportClient(std::string& path);

    private:
        QSqlDatabase&     db_;
        DataLayer data_layer_;
        std::shared_ptr<jp::Employee> authorized_employee_;
    };
}

#endif
