
#ifndef APPBUSINESSLOGIC_H
#define APPBUSINESSLOGIC_H

#include "windows/authorization_window.h"
#include "windows/main_window.h"
#include "windows/administrator_window.h"
#include "controller.h"
#include "windows/top_selling_books.h"

namespace jp
{
    class AppBusinessLogic: public QObject
    {
        Q_OBJECT

    public:

        AppBusinessLogic(AuthorizationWindow& auth_window_, QSqlDatabase& db);
        void loaderOrder(int status = 1) noexcept;
        void loaderBook() noexcept;
        void loaderEmployee() noexcept;
        void loaderOrderWindow() noexcept;
        void loaderBookWindow() noexcept;
        void loaderPropertyBookWindow() noexcept;

        void authorizationWindow(std::string login, std::string password);
        void logoutWindow();

        void searchOrderWindow(QString str);
        void searchBookWindow(QString str);

        void addBookWindow(QString title, QString author, QString genre,
                           QString publisher, QString year, QString price);
        void addOrderWindow(QString number, QString address, QString sum, QTableWidget* comboBook);
        void addEmployeeWindow(std::shared_ptr<jp::Employee> employee);
        void addClientWindow(std::string name, std::string number);
        void addArrivalWindow(QString title, QString count);
        void addPublisherWindow(std::shared_ptr<jp::Publisher> publisher);
        void addAuthorWindow(std::shared_ptr<jp::Author> author);
        void addGenreWindow(std::shared_ptr<jp::Genre> genre);

        void changeBookWindow(std::shared_ptr<jp::Book> book);
        void changeOrderWindow(std::shared_ptr<jp::Order> order, int status);
        void changeEmployeeWindow(std::shared_ptr<jp::Employee> employee);
        void changePublisherWindow(std::shared_ptr<jp::Publisher> publisher);
        void changeAuthorWindow(std::shared_ptr<jp::Author> author);
        void changeGenreWindow(std::shared_ptr<jp::Genre> genre);

        void reportSalesWindow(std::string path);
        void reportSellingBooksWindow(std::string path);
        void reportSellingBooksMonthWindow(std::string path);
        void reportClientWindow(std::string path);

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
        Controller controller_;
    };
}


#endif
