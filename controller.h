
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "logic.h"
#include "qsqldatabase.h"
#include "windows/authorization_window.h"
#include "windows/main_window.h"
#include "windows/administrator_window.h"
#include "windows/top_selling_books.h"

namespace jp
{
    class Controller: public QObject
    {
        Q_OBJECT

    public:

        Controller(AuthorizationWindow& auth_window_, QSqlDatabase& db);
        void loaderOrder(int status = 4);
        void loaderBook();
        void loaderEmployee();
        void loaderOrderWindow();
        void loaderBookWindow();
        void loaderPropertyBookWindow();

        void authorizationWindow(std::string login, std::string password);
        void logoutWindow();

        void searchOrderWindow(std::string str);
        void searchBookWindow(std::string str);

        void addBookWindow(std::shared_ptr<Book> book);
        void addOrderWindow(std::string number, std::string address, std::string sum, std::list<jp::Book> books);
        void addEmployeeWindow(std::shared_ptr<jp::Employee> employee);
        void addClientWindow(std::string name, std::string number);
        void addArrivalWindow(std::shared_ptr<Book>, std::string count);
        void addPublisherWindow(std::shared_ptr<jp::Publisher> publisher);
        void addAuthorWindow(std::shared_ptr<jp::Author> author);
        void addGenreWindow(std::shared_ptr<jp::Genre> genre);

        void changeBookWindow(std::shared_ptr<jp::Book> book);
        void changeOrderWindow(std::shared_ptr<jp::Order> order, int status);
        void changeEmployeeWindow(std::shared_ptr<jp::Employee> employee);
        void changePublisherWindow(std::shared_ptr<jp::Publisher> publisher);
        void changeAuthorWindow(std::shared_ptr<jp::Author> author);
        void changeGenreWindow(std::shared_ptr<jp::Genre> genre);

    private:
        AuthorizationWindow& authorization_window_;
        SalesThreeMonths     sales_three_monts_;
        TopSellingBooks      top_selling_books_;
        TopSellingBooksMonth top_selling_books_month_;
        TopClient            top_client_;
        AddBookWindow        add_book_window_;
        AddArrivalWindow     add_arrival_window_;
        OrderWindow          order_window_;
        EmployeeWindow       employee_window_;
        AddEmployeeWindow    add_employee_window_;
        AddClientWindow      add_client_window_;
        AddOrderWindow       add_order_window_;
        MainWindow           window_;
        AdministratorWindow  admin_window_;
        QSqlDatabase& db_;
        Logic logic_;
        int oldStatus_;
    };
}

#endif
