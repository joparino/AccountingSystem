
#include "app_business_logic.h"
#include "qmessagebox.h"

jp::AppBusinessLogic::AppBusinessLogic(AuthorizationWindow& auth_window_, QSqlDatabase& db):
    authorization_window_(auth_window_),
    add_order_window_(add_client_window_),
    window_(order_window_, add_order_window_, add_book_window_,
            add_arrival_window_, sales_three_monts_, top_selling_books_,
            top_selling_books_month_, top_client_),
    admin_window_(add_employee_window_, employee_window_),
    db_(db),
    controller_(db)
{
    QObject::connect(&window_, &MainWindow::searchOrderTriggered,
                     this,     &jp::AppBusinessLogic::searchOrderWindow);
    QObject::connect(&window_, &MainWindow::searchBookTriggered,
                     this,     &jp::AppBusinessLogic::searchBookWindow);
    QObject::connect(&window_, &MainWindow::filterTriggered,
                     this,     &jp::AppBusinessLogic::loaderOrder);
    QObject::connect(&window_, &MainWindow::exportTopSalesTriggered,
                     this,     &jp::AppBusinessLogic::reportSalesWindow);
    QObject::connect(&window_, &MainWindow::exportTopSellingTriggered,
                     this,     &jp::AppBusinessLogic::reportSellingBooksWindow);
    QObject::connect(&window_, &MainWindow::exportTopSellingMonthTriggered,
                     this,     &jp::AppBusinessLogic::reportSellingBooksMonthWindow);
    QObject::connect(&window_, &MainWindow::exportTopClientTriggered,
                     this,     &jp::AppBusinessLogic::reportClientWindow);
    QObject::connect(&window_, &MainWindow::logoutTriggered,
                     this,     &jp::AppBusinessLogic::logoutWindow);

    QObject::connect(&add_client_window_, &AddClientWindow::addClientTriggered,
                     this,                &jp::AppBusinessLogic::addClientWindow);

    QObject::connect(&add_book_window_, &AddBookWindow::addBookTriggered,
                     this,              &jp::AppBusinessLogic::addBookWindow);
    QObject::connect(&add_book_window_, &AddBookWindow::changeBookTriggered,
                     this,              &jp::AppBusinessLogic::changeBookWindow);

    QObject::connect(&add_arrival_window_, &AddArrivalWindow::addArrivalTriggered,
                     this,                 &jp::AppBusinessLogic::addArrivalWindow);

    QObject::connect(&add_order_window_, &AddOrderWindow::addOrder,
                     this,               &jp::AppBusinessLogic::addOrderWindow);

    QObject::connect(&order_window_, &OrderWindow::hasOrderChangedTriggered,
                     this,           &jp::AppBusinessLogic::changeOrderWindow);

    QObject::connect(&employee_window_, &EmployeeWindow::employeeChanged,
                     this,              &jp::AppBusinessLogic::changeEmployeeWindow);

    QObject::connect(&add_employee_window_, &AddEmployeeWindow::addEmployeeTriggered,
                     this,                  &jp::AppBusinessLogic::addEmployeeWindow);

    QObject::connect(&admin_window_, &AdministratorWindow::logoutTriggered,
                     this,           &jp::AppBusinessLogic::logoutWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::addPublisherTriggered,
                     this,           &jp::AppBusinessLogic::addPublisherWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::addAuthorTriggered,
                     this,           &jp::AppBusinessLogic::addAuthorWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::addGenreTriggered,
                     this,           &jp::AppBusinessLogic::addGenreWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::changePublisherTriggered,
                     this,           &jp::AppBusinessLogic::changePublisherWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::changeAuthorTriggered,
                     this,           &jp::AppBusinessLogic::changeAuthorWindow);
    QObject::connect(&admin_window_, &AdministratorWindow::changeGenreTriggered,
                     this,           &jp::AppBusinessLogic::changeGenreWindow);
}


void jp::AppBusinessLogic::loaderOrder(int status) noexcept
{
    window_.clearOrder();
    for (auto& order : controller_.data_layer_.orders_)
    {
        if (order->status->id == status)
        {
            window_.addRowOrder(order);
        }
    }
}


void jp::AppBusinessLogic::loaderBook() noexcept
{
    for (auto& book : controller_.data_layer_.books_)
    {
        window_.addRowBook(book);
    }
}


void jp::AppBusinessLogic::loaderEmployee() noexcept
{
    for (auto& employee : controller_.data_layer_.employees_)
    {
        admin_window_.addRowEmployee(employee);
    }
}


void jp::AppBusinessLogic::loaderOrderWindow() noexcept
{
    add_order_window_.clearBooks();
    for (auto& book : controller_.data_layer_.books_)
    {
        add_order_window_.addTitleBook(book->title, book);
    }

    order_window_.clearStatus();
    for (auto& status : controller_.data_layer_.statuses_)
    {
        order_window_.addStatus(status);
    }
}


void jp::AppBusinessLogic::loaderBookWindow() noexcept
{
    add_book_window_.clear();
    for (auto& author : controller_.data_layer_.authors_)
    {
        add_book_window_.addItemAuthor(author);
    }

    for (auto& publisher : controller_.data_layer_.publishers_)
    {
        add_book_window_.addItemPublisher(publisher);
    }

    for (auto& genre : controller_.data_layer_.genres_)
    {
        add_book_window_.addItemGenre(genre);
    }

    add_arrival_window_.clear();
    for (auto& book : controller_.data_layer_.books_)
    {
        add_arrival_window_.addItemBook(book);
    }
}


void jp::AppBusinessLogic::loaderPropertyBookWindow() noexcept
{
    admin_window_.clearPropertyBook();
    for (auto& author : controller_.data_layer_.authors_)
    {
        admin_window_.addRowAuthor(author);
    }

    for (auto& publisher : controller_.data_layer_.publishers_)
    {
        admin_window_.addRowPublisher(publisher);
    }

    for (auto& genre : controller_.data_layer_.genres_)
    {
        admin_window_.addRowGenre(genre);
    }
}


void jp::AppBusinessLogic::authorizationWindow(std::string login, std::string password)
{
    int result = controller_.authorization(login, password);
    if (result == 1)
    {
        loaderOrder();
        loaderOrderWindow();
        loaderBook();
        loaderBookWindow();
        sales_three_monts_.setSales(controller_.getSales());
        top_selling_books_.setTopSellingBooks(controller_.getTopBook());
        top_selling_books_month_.setTopSellingBooksMonth(controller_.getTopBookMonth());
        top_client_.setTopClient(controller_.getTopClient());
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


void jp::AppBusinessLogic::logoutWindow()
{
    authorization_window_.show();
}


void jp::AppBusinessLogic::searchOrderWindow(QString str)
{
    DataLayer::Orders orders = controller_.searchOrder(str);
    if (str == "")
    {
        window_.clearOrder();
        loaderOrder();
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


void jp::AppBusinessLogic::searchBookWindow(QString str)
{
    DataLayer::Books books = controller_.searchBook(str);
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


void jp::AppBusinessLogic::addBookWindow(std::shared_ptr<jp::Book> book)
{
    if (controller_.addBook(book))
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


void jp::AppBusinessLogic::addOrderWindow(QString number, QString address, QString sum, QTableWidget* comboBook)
{
    if (controller_.addOrder(number, address, sum, comboBook))
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


void jp::AppBusinessLogic::addEmployeeWindow(std::shared_ptr<jp::Employee> employee)
{
    if (controller_.addEmployee(employee))
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


void jp::AppBusinessLogic::addClientWindow(std::string name, std::string number)
{
    if (controller_.addClient(name, number))
    {
        add_client_window_.reject();
    }
    else
    {
        QMessageBox::information(nullptr, "Ошибка", "Не удалось добавить пользователя");
    }
}


void jp::AppBusinessLogic::addArrivalWindow(std::shared_ptr<jp::Book> book, QString count)
{
    if (controller_.addArrival(book, count))
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


void jp::AppBusinessLogic::addPublisherWindow(std::shared_ptr<jp::Publisher> publisher)
{
    controller_.addPublisher(publisher);
}


void jp::AppBusinessLogic::addAuthorWindow(std::shared_ptr<jp::Author> author)
{
    controller_.addAuthor(author);
}


void jp::AppBusinessLogic::addGenreWindow(std::shared_ptr<jp::Genre> genre)
{
    controller_.addGenre(genre);
}


void jp::AppBusinessLogic::changeBookWindow(std::shared_ptr<jp::Book> book)
{
    if (controller_.changeBook(book))
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


void jp::AppBusinessLogic::changeOrderWindow(std::shared_ptr<jp::Order> order, int status)
{
    if (controller_.changeOrder(order, status))
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


void jp::AppBusinessLogic::changeEmployeeWindow(std::shared_ptr<jp::Employee> employee)
{
    if (controller_.changeEmployee(employee))
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


void jp::AppBusinessLogic::changePublisherWindow(std::shared_ptr<jp::Publisher> publisher)
{
    controller_.changePublisher(publisher);
}


void jp::AppBusinessLogic::changeAuthorWindow(std::shared_ptr<jp::Author> author)
{
    controller_.changeAuthor(author);
}


void jp::AppBusinessLogic::changeGenreWindow(std::shared_ptr<jp::Genre> genre)
{
    controller_.changeGenre(genre);
}


void jp::AppBusinessLogic::reportSalesWindow(std::string path)
{
    controller_.reportSales(path);
}


void jp::AppBusinessLogic::reportSellingBooksWindow(std::string path)
{
    controller_.reportSellingBooks(path);
}


void jp::AppBusinessLogic::reportSellingBooksMonthWindow(std::string path)
{
    controller_.reportSellingBooksMonth(path);
}


void jp::AppBusinessLogic::reportClientWindow(std::string path)
{
    controller_.reportClient(path);
}
