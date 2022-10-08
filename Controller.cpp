
#include "controller.h"
#include "qmessagebox.h"

jp::Controller::Controller(AuthorizationWindow& auth_window_, QSqlDatabase& db):
    authorization_window_(auth_window_),
    add_order_window_(add_client_window_),
    window_(order_window_, add_order_window_, add_book_window_,
            add_arrival_window_, sales_three_monts_, top_selling_books_,
            top_selling_books_month_, top_client_),
    admin_window_(add_employee_window_, employee_window_),
    db_(db),
    logic_(db)
{
    QObject::connect(&window_, &MainWindow::searchOrderTriggered,
                     this,     &jp::Controller::searchOrderWindow);
    QObject::connect(&window_, &MainWindow::searchBookTriggered,
                     this,     &jp::Controller::searchBookWindow);
    QObject::connect(&window_, &MainWindow::filterTriggered,
                     this,     &jp::Controller::loaderOrder);

    QObject::connect(&window_, &MainWindow::logoutTriggered,
                     this,     &jp::Controller::logoutWindow);

    QObject::connect(&add_client_window_, &AddClientWindow::addClientTriggered,
                     this,                &jp::Controller::addClientWindow);

    QObject::connect(&add_book_window_, &AddBookWindow::addBookTriggered,
                     this,              &jp::Controller::addBookWindow);
    QObject::connect(&add_book_window_, &AddBookWindow::changeBookTriggered,
                     this,              &jp::Controller::changeBookWindow);

    QObject::connect(&add_arrival_window_, &AddArrivalWindow::addArrivalTriggered,
                     this,                 &jp::Controller::addArrivalWindow);

    QObject::connect(&add_order_window_, &AddOrderWindow::addOrder,
                     this,               &jp::Controller::addOrderWindow);

    QObject::connect(&order_window_, &OrderWindow::hasOrderChangedTriggered,
                     this,           &jp::Controller::changeOrderWindow);

    QObject::connect(&employee_window_, &EmployeeWindow::employeeChanged,
                     this,              &jp::Controller::changeEmployeeWindow);

    QObject::connect(&add_employee_window_, &AddEmployeeWindow::addEmployeeTriggered,
                     this,                  &jp::Controller::addEmployeeWindow);

    QObject::connect(&admin_window_, &AdministratorWindow::logoutTriggered,
                     this,           &jp::Controller::logoutWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::addPublisherTriggered,
                     this,           &jp::Controller::addPublisherWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::addAuthorTriggered,
                     this,           &jp::Controller::addAuthorWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::addGenreTriggered,
                     this,           &jp::Controller::addGenreWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::changePublisherTriggered,
                     this,           &jp::Controller::changePublisherWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::changeAuthorTriggered,
                     this,           &jp::Controller::changeAuthorWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::changeGenreTriggered,
                     this,           &jp::Controller::changeGenreWindow);
}


void jp::Controller::loaderOrder(int status)
{
    window_.clearOrder();
    oldStatus_ = window_.setIndex();
    for (auto& order : logic_.data_layer_.orders_)
    {
        if (order->status->id == oldStatus_)
        {
            window_.addRowOrder(order);
        }
    }
}


void jp::Controller::loaderBook()
{
    window_.clearBook();
    for (auto& book : logic_.data_layer_.books_)
    {
        window_.addRowBook(book);
    }
}


void jp::Controller::loaderEmployee()
{
    admin_window_.clearEmployee();
    for (auto& employee : logic_.data_layer_.employees_)
    {
        if (employee != logic_.authorized_employee_)
        {
            admin_window_.addRowEmployee(employee);
        }
    }
}


void jp::Controller::loaderOrderWindow()
{
    add_order_window_.clearBooks();
    for (auto& book : logic_.data_layer_.books_)
    {
        add_order_window_.addTitleBook(book->title, book);
    }

    order_window_.clearStatus();
    for (auto& status : logic_.data_layer_.statuses_)
    {
        order_window_.addStatus(status);
    }
}


void jp::Controller::loaderBookWindow()
{
    add_book_window_.clear();
    for (auto& author : logic_.data_layer_.authors_)
    {
        add_book_window_.addItemAuthor(author);
    }

    for (auto& publisher : logic_.data_layer_.publishers_)
    {
        add_book_window_.addItemPublisher(publisher);
    }

    for (auto& genre : logic_.data_layer_.genres_)
    {
        add_book_window_.addItemGenre(genre);
    }

    add_arrival_window_.clear();
    for (auto& book : logic_.data_layer_.books_)
    {
        add_arrival_window_.addItemBook(book);
    }
}


void jp::Controller::loaderPropertyBookWindow()
{
    admin_window_.clearPropertyBook();
    for (auto& author : logic_.data_layer_.authors_)
    {
        admin_window_.addRowAuthor(author);
    }

    for (auto& publisher : logic_.data_layer_.publishers_)
    {
        admin_window_.addRowPublisher(publisher);
    }

    for (auto& genre : logic_.data_layer_.genres_)
    {
        admin_window_.addRowGenre(genre);
    }
}


void jp::Controller::authorizationWindow(std::string login, std::string password)
{
    int result = logic_.authorization(login, password);
    if (result == 1)
    {
        loaderOrder();
        loaderOrderWindow();
        loaderBook();
        loaderBookWindow();
        sales_three_monts_.setSales(logic_.getSales());
        top_selling_books_.setTopSellingBooks(logic_.getTopBook());
        top_selling_books_month_.setTopSellingBooksMonth(logic_.getTopBookMonth());
        top_client_.setTopClient(logic_.getTopClient());
        window_.show();
        authorization_window_.hide();
    }
    else if (result == 2)
    {
        loaderEmployee();
        loaderPropertyBookWindow();
        admin_window_.show();
        authorization_window_.hide();
    }
    else
    {
        QMessageBox::critical(nullptr, "Не удалось авторизоваться", "Логин или пароль неправильный");
    }
}


void jp::Controller::logoutWindow()
{
    authorization_window_.show();
}


void jp::Controller::searchOrderWindow(std::string str)
{
    DataLayer::Orders orders = logic_.searchOrder(str);
    if (str == "")
    {
        window_.clearOrder();
        loaderOrder(oldStatus_);
    }
    else if (!orders.empty())
    {
        window_.clearOrder();
        for (auto& order : orders)
        {
            window_.addRowOrder(order);
        }
    }
    else
    {
        QMessageBox::information(0, "Не найдено", "Такого пользователя нет!");
    }
}


void jp::Controller::searchBookWindow(std::string str)
{
    DataLayer::Books books = logic_.searchBook(str);
    if (str == "")
    {
        window_.clearBook();
        loaderBook();
    }
    else if (!books.empty())
    {
        window_.clearBook();
        for (auto& book : books)
        {
            window_.addRowBook(book);
        }
    }
    else
    {
        QMessageBox::information(nullptr, "Не найдено", "Такой книги нет!");
    }
}


void jp::Controller::addBookWindow(std::shared_ptr<jp::Book> book)
{
    if (logic_.addBook(book))
    {
        window_.clearBook();
        loaderBook();
        add_book_window_.reject();
    }
    else
    {
        QMessageBox::information(nullptr, "Ошибка", "Не удалось добавить книгу");
    }
}


void jp::Controller::addOrderWindow(std::string number, std::string address, std::string sum, std::list<jp::Book> books)
{
    if (logic_.addOrder(number, address, sum, books))
    {
        window_.clearOrder();
        loaderOrder();
        add_order_window_.reject();
    }
    else
    {
        QMessageBox::information(nullptr, "Ошибка", "Не удалось добавить заказ");
    }
}


void jp::Controller::addEmployeeWindow(std::shared_ptr<jp::Employee> employee)
{
    if (logic_.addEmployee(employee))
    {
        admin_window_.clearEmployee();
        loaderEmployee();
        add_employee_window_.reject();
    }
    else
    {
        QMessageBox::information(nullptr, "Ошибка", "Не удалось добавить сотрудника");
    }
}


void jp::Controller::addClientWindow(std::string name, std::string number)
{
    if (logic_.addClient(name, number))
    {
        add_client_window_.reject();
    }
    else
    {
        QMessageBox::information(nullptr, "Ошибка", "Не удалось добавить пользователя");
    }
}


void jp::Controller::addArrivalWindow(std::shared_ptr<jp::Book> book, std::string count)
{
    if (logic_.addArrival(book, count))
    {
        window_.clearBook();
        loaderBook();
        add_arrival_window_.reject();
    }
    else
    {
        QMessageBox::information(nullptr, "Ошибка", "Что-то пошло не так");
    }
}


void jp::Controller::addPublisherWindow(std::shared_ptr<jp::Publisher> publisher)
{
    logic_.addPublisher(publisher);
}


void jp::Controller::addAuthorWindow(std::shared_ptr<jp::Author> author)
{
    logic_.addAuthor(author);
}


void jp::Controller::addGenreWindow(std::shared_ptr<jp::Genre> genre)
{
    logic_.addGenre(genre);
}


void jp::Controller::changeBookWindow(std::shared_ptr<jp::Book> book)
{
    if (logic_.changeBook(book))
    {
        window_.clearBook();
        loaderBook();
        add_book_window_.reject();
    }
    else
    {
        QMessageBox::information(nullptr, "Ошибка", "Не удалось изменить книгу");
    }
}


void jp::Controller::changeOrderWindow(std::shared_ptr<jp::Order> order, int status)
{
    if (logic_.changeOrder(order, status))
    {
        window_.clearOrder();
        loaderOrder();
        order_window_.reject();
    }
    else
    {
        QMessageBox::information(nullptr, "Ошибка", "Не удалось изменить заказ");
    }
}


void jp::Controller::changeEmployeeWindow(std::shared_ptr<jp::Employee> employee)
{
    if (logic_.changeEmployee(employee))
    {
        admin_window_.clearEmployee();
        loaderEmployee();
        employee_window_.reject();
    }
    else
    {
        QMessageBox::information(nullptr, "Ошибка", "Не удалось изменить сотрудника");
    }
}


void jp::Controller::changePublisherWindow(std::shared_ptr<jp::Publisher> publisher)
{
    logic_.changePublisher(publisher);
}


void jp::Controller::changeAuthorWindow(std::shared_ptr<jp::Author> author)
{
    logic_.changeAuthor(author);
}


void jp::Controller::changeGenreWindow(std::shared_ptr<jp::Genre> genre)
{
    logic_.changeGenre(genre);
}
