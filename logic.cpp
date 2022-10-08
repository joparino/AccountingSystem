
#include <set>
#include <fstream>
#include <QMessageBox>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDate>
#include "logic.h"

jp::Logic::Logic(QSqlDatabase& db):
    db_(db),
    data_layer_(db_)
{}


int jp::Logic::authorization(std::string& login, std::string& password)
{
    QSqlQuery queryAuthorization(db_);
    queryAuthorization.prepare("SELECT role, password, is_active FROM employee\
                                WHERE login = :login");
    queryAuthorization.bindValue(":login", QString::fromStdString(login));
    queryAuthorization.exec();

    DataLayer::Employees::iterator employee = std::find(data_layer_.employees_.begin(), data_layer_.employees_.end(),
                                                        login);

    std::string passwordHash = QString(QCryptographicHash::hash((password.c_str()),QCryptographicHash::Md5).toHex()).toStdString();

    while(queryAuthorization.next())
    {
        if (passwordHash == queryAuthorization.value(1).toString().toStdString() && queryAuthorization.value(2).toBool() == true)
        {
            if (queryAuthorization.value(0).toBool() == true)
            {
                authorized_employee_ = *employee;
                return 1;
            }
            else
            {
                authorized_employee_ = *employee;
                return 2;
            }
        }
    }

    return 0;
}


jp::DataLayer::Orders jp::Logic::searchOrder(std::string& str)
{
    jp::DataLayer::Orders orders;
    if (data_layer_.bf_.check(str.c_str()))
    {
        for (auto& order : data_layer_.orders_)
        {
            if (order->client->name == str)
            {
               orders.push_back(order);
            }
        }
    }
    return orders;
}


jp::DataLayer::Books jp::Logic::searchBook(std::string& str)
{
    jp::DataLayer::Books books;
    if (data_layer_.bfb_.check(str.c_str()))
    {
        for (auto& book : data_layer_.books_)
        {
            if (book->title == str)
            {
               books.push_back(book);
            }
        }
    }
    return books;
}


bool jp::Logic::addBook(std::shared_ptr<jp::Book> book)
{
    QSqlQuery queryAddBook(db_);
    queryAddBook.prepare("INSERT INTO book (title, id_author, id_genre, id_publisher, year, price, count)\
                          VALUES (:title, :id_author, :id_genre, :id_publisher, :year, :price, 0)");
    queryAddBook.bindValue(":title", QString::fromStdString(book->title));
    queryAddBook.bindValue(":id_author", book->author->id);
    queryAddBook.bindValue(":id_genre", book->genre->id);
    queryAddBook.bindValue(":id_publisher", book->publisher->id);
    queryAddBook.bindValue(":year", QString::fromStdString(book->year));
    queryAddBook.bindValue(":price", book->price);

    if (queryAddBook.exec())
    {
        data_layer_.books_.push_back(book);
        data_layer_.bfb_.insert(book->title.c_str());

        return true;
    }
    return false;
}


bool jp::Logic::addOrder(std::string& number, std::string& address, std::string& sum, std::list<jp::Book> books)
{
    DataLayer::Clients::iterator clientFound  = std::find(data_layer_.clients_.begin(), data_layer_.clients_.end(),
                                                          number);
    if (clientFound != data_layer_.clients_.end())
    {
        QSqlQuery queryAddPurchase(db_);
        queryAddPurchase.prepare("INSERT INTO purschase (id_employee, id_client, id_status, address, date, sum)\
                                 VALUES (:id_employee, :id_client, 4, :address, :date, :sum)\
                                 RETURNING id");
        queryAddPurchase.bindValue(":id_employee", authorized_employee_->id);
        queryAddPurchase.bindValue(":id_client", (*clientFound)->id);
        queryAddPurchase.bindValue(":address", QString::fromStdString(address));
        queryAddPurchase.bindValue(":date", QDate::currentDate().toString("dd.MM.yyyy"));
        queryAddPurchase.bindValue(":sum", QString::fromStdString(sum));

        if (queryAddPurchase.exec())
        {
            QString idPurchase = "";
            while(queryAddPurchase.next())
            {
                idPurchase = queryAddPurchase.value(0).toString();
            }

            DataLayer::Statuses::iterator status = std::find(data_layer_.statuses_.begin(), data_layer_.statuses_.end(),
                                                             4);

            std::shared_ptr<jp::Order> order(new jp::Order);

            order->id = idPurchase.toInt();
            order->client = *clientFound;
            order->status = *status;
            order->employee = authorized_employee_->id;
            order->address = address;
            order->date = QDate::currentDate().toString("dd.MM.yyyy").toStdString();
            order->sum = std::stof(sum);

            for (auto& book : books)
            {
                QSqlQuery queryAddInBasket(db_);
                queryAddInBasket.prepare("INSERT INTO Basket (id_book, id_purschase, count, sale_price)\
                                          VALUES (:id, :id_purchase, :count, :price)");
                queryAddInBasket.bindValue(":id", book.id);
                queryAddInBasket.bindValue(":title", QString::fromStdString(book.title));
                queryAddInBasket.bindValue(":count", book.count);
                queryAddInBasket.bindValue(":id_purchase", idPurchase);
                queryAddInBasket.bindValue(":price", book.price);
                if (!queryAddInBasket.exec())
                {
                    return false;
                }

                QSqlQuery queryCountBook(db_);
                queryCountBook.prepare("UPDATE Book SET count = count - :count WHERE id = :id");
                queryCountBook.bindValue(":id", book.id);
                queryCountBook.bindValue(":title", QString::fromStdString(book.title));
                queryCountBook.bindValue(":count", book.count);
                if (!queryCountBook.exec())
                {
                    return false;
                }

                auto bookFound = std::find(data_layer_.books_.begin(), data_layer_.books_.end(), book.id);
                (*bookFound)->count -= book.count;
                order->books.push_back(book);
            }

            data_layer_.orders_.push_back(order);
            data_layer_.bf_.insert(order->client->name.c_str());

            return true;
        }
    }
    return false;
}


bool jp::Logic::addEmployee(std::shared_ptr<jp::Employee> employee)
{
    QSqlQuery queryAddEmployee(db_);
    queryAddEmployee.prepare("INSERT INTO employee (first_name, surname, patronymic, login, password, role, is_active)\
                              VALUES (:first_name, :surname, :patronymic, :login, :password, :role, :is_active)");
    queryAddEmployee.bindValue(":first_name", QString::fromStdString(employee->firstName));
    queryAddEmployee.bindValue(":surname", QString::fromStdString(employee->surname));
    queryAddEmployee.bindValue(":patronymic", QString::fromStdString(employee->patronymic));
    queryAddEmployee.bindValue(":login", QString::fromStdString(employee->login));
    queryAddEmployee.bindValue(":password", QString::fromStdString(employee->password));
    queryAddEmployee.bindValue(":role", employee->role);
    queryAddEmployee.bindValue(":is_active", employee->isActive);

    if (queryAddEmployee.exec())
    {
        employee->id = data_layer_.employees_.back()->id + 1;
        data_layer_.employees_.push_back(employee);

        return true;
    }
    return false;
}


bool jp::Logic::addClient(std::string& name, std::string& number)
{
    DataLayer::Clients::iterator clientFound  = std::find(data_layer_.clients_.begin(),
                                                          data_layer_.clients_.end(), number);
    if (clientFound == data_layer_.clients_.end())
    {
        QSqlQuery queryAddClient(db_);
        queryAddClient.prepare("INSERT INTO client (name, phone_number) VALUES (:name, :number)\
                                RETURNING id");
        queryAddClient.bindValue(":name", QString::fromStdString(name));
        queryAddClient.bindValue(":number", QString::fromStdString(number));

        if (queryAddClient.exec())
        {
            int idClient = 0;
            while(queryAddClient.next())
            {
                idClient = queryAddClient.value(0).toInt();
            }
            if (idClient != 0)
            {
                jp::Client client;
                client.id = idClient;
                client.name = name;
                client.phoneNumber = number;
                data_layer_.clients_.push_back(std::make_shared<jp::Client>(client));
                return true;
            }
        }
    }
    return false;
}


bool jp::Logic::addArrival(std::shared_ptr<jp::Book> book, std::string& count)
{
    QSqlQuery queryAddArrival(db_);
    queryAddArrival.prepare("UPDATE book SET count = count + :count WHERE id = :id");
    queryAddArrival.bindValue(":id", book->id);
    queryAddArrival.bindValue(":count", QString::fromStdString(count));

    if (queryAddArrival.exec())
    {
        book->count += std::atoi(count.c_str());
        return true;
    }
    return false;
}


bool jp::Logic::addPublisher(std::shared_ptr<jp::Publisher> publisher)
{
    QSqlQuery queryAddPublisher(db_);
    queryAddPublisher.prepare("INSERT INTO publisher (name_of_publisher) VALUES (:name)");
    queryAddPublisher.bindValue(":name", QString::fromStdString(publisher->name));
    if (queryAddPublisher.exec())
    {
        publisher->id = data_layer_.publishers_.back()->id + 1;
        data_layer_.publishers_.push_back(publisher);
        return true;
    }
    return false;
}


bool jp::Logic::addAuthor(std::shared_ptr<jp::Author> author)
{
    QSqlQuery queryAddAuthor(db_);
    queryAddAuthor.prepare("INSERT INTO author (name_of_author) VALUES (:name)");
    queryAddAuthor.bindValue(":name", QString::fromStdString(author->name));
    if (queryAddAuthor.exec())
    {
        author->id = data_layer_.authors_.back()->id + 1;
        data_layer_.authors_.push_back(author);
        return true;
    }
    return false;
}


bool jp::Logic::addGenre(std::shared_ptr<jp::Genre> genre)
{
    QSqlQuery queryAddGenre(db_);
    queryAddGenre.prepare("INSERT INTO genre (name_of_genre) VALUES (:name)");
    queryAddGenre.bindValue(":name", QString::fromStdString(genre->name));
    if (queryAddGenre.exec())
    {
        genre->id = data_layer_.genres_.back()->id + 1;
        data_layer_.genres_.push_back(genre);
        return true;
    }
    return false;
}


bool jp::Logic::changeBook(std::shared_ptr<jp::Book> book)
{
    QSqlQuery queryChangeBook(db_);
    queryChangeBook.prepare("UPDATE book SET title = :title, id_author = :id_author, id_genre = :id_genre,\
                             id_publisher = :id_publisher, year = :year, price = :price WHERE id = :id");
    queryChangeBook.bindValue(":id", book->id);
    queryChangeBook.bindValue(":title", QString::fromStdString(book->title));
    queryChangeBook.bindValue(":id_author", book->author->id);
    queryChangeBook.bindValue(":id_genre", book->genre->id);
    queryChangeBook.bindValue(":id_publisher", book->publisher->id);
    queryChangeBook.bindValue(":year", QString::fromStdString(book->year));
    queryChangeBook.bindValue(":price",book->price);
    if (queryChangeBook.exec())
    {
        return true;
    }
    return false;
}


bool jp::Logic::changeOrder(std::shared_ptr<jp::Order> order, int status)
{
    DataLayer::Statuses::iterator statusFound  = std::find(data_layer_.statuses_.begin(),
                                                           data_layer_.statuses_.end(), status);
    order->status = *statusFound;

    QSqlQuery queryChangeClient(db_);
    queryChangeClient.prepare("UPDATE client SET name = :name, phone_number = :number WHERE id = :id_client");
    queryChangeClient.bindValue(":id_client", order->client->id);
    queryChangeClient.bindValue(":name", QString::fromStdString(order->client->name));
    queryChangeClient.bindValue(":number", QString::fromStdString(order->client->phoneNumber));

    if (queryChangeClient.exec())
    {
        QSqlQuery queryChangeOrder(db_);
        queryChangeOrder.prepare("UPDATE purschase SET address = :address, id_status = :status WHERE id = :id_order");
        queryChangeOrder.bindValue(":id_order", order->id);
        queryChangeOrder.bindValue(":address", QString::fromStdString(order->address));
        queryChangeOrder.bindValue(":status", order->status->id);

        if (queryChangeOrder.exec())
        {
            return true;
        }
    }

    return false;
}


bool jp::Logic::changeEmployee(std::shared_ptr<jp::Employee> employee)
{
    if (employee == authorized_employee_)
    {
        if (employee->isActive != authorized_employee_->isActive)
        {
            employee->isActive = authorized_employee_->isActive;
        }
    }
    QSqlQuery queryChangeEmployee(db_);
    queryChangeEmployee.prepare("UPDATE employee SET first_name = :firstName, surname = :surname, patronymic = :patronymic,\
                                 login = :login, password = :password, role = :role, is_active = :isActive \
                                 WHERE id = :id");
    queryChangeEmployee.bindValue(":id", employee->id);
    queryChangeEmployee.bindValue(":firstName", QString::fromStdString(employee->firstName));
    queryChangeEmployee.bindValue(":surname", QString::fromStdString(employee->surname));
    queryChangeEmployee.bindValue(":patronymic", QString::fromStdString(employee->patronymic));
    queryChangeEmployee.bindValue(":login", QString::fromStdString(employee->login));
    queryChangeEmployee.bindValue(":password", QString::fromStdString(employee->password));
    queryChangeEmployee.bindValue(":role", employee->role);
    queryChangeEmployee.bindValue(":isActive", employee->isActive);
    if (queryChangeEmployee.exec())
    {
        return true;
    }
    return false;
}


bool jp::Logic::changePublisher(std::shared_ptr<jp::Publisher> publisher)
{
    QSqlQuery queryChangePublisher(db_);
    queryChangePublisher.prepare("UPDATE publisher SET name_of_Publisher = :name WHERE id = :id");
    queryChangePublisher.bindValue(":id", publisher->id);
    queryChangePublisher.bindValue(":name", QString::fromStdString(publisher->name));

    if (queryChangePublisher.exec())
    {
        return true;
    }
    return false;
}


bool jp::Logic::changeAuthor(std::shared_ptr<jp::Author> author)
{
    QSqlQuery queryChangeAuthor(db_);
    queryChangeAuthor.prepare("UPDATE author SET name_of_author = :name WHERE id = :id");
    queryChangeAuthor.bindValue(":id", author->id);
    queryChangeAuthor.bindValue(":name", QString::fromStdString(author->name));

    if (queryChangeAuthor.exec())
    {
        return true;
    }
    return false;
}


bool jp::Logic::changeGenre(std::shared_ptr<jp::Genre> genre)
{
    QSqlQuery queryChangeGenre(db_);
    queryChangeGenre.prepare("UPDATE genre SET name_of_genre = :name WHERE id = :id");
    queryChangeGenre.bindValue(":id", genre->id);
    queryChangeGenre.bindValue(":name", QString::fromStdString(genre->name));

    if (queryChangeGenre.exec())
    {
        return true;
    }
    return false;
}


std::vector<float> jp::Logic::getSales()
{
    float sum = 0;
    std::vector<float> sales;

    QDate firstDate = QDate::currentDate();
    QDate secondDate = firstDate.addMonths(-1);
    QDate thirstDate = secondDate.addMonths(-1);

    QString date = QString::number(firstDate.year()) + "." + QString::number(firstDate.month()) + ".01";
    QString lastDate = QString::number(firstDate.year()) + "." + QString::number(firstDate.month()) + "." +
                                   QString::number(firstDate.daysInMonth());

    QSqlQuery queryGetFirstSales(db_);
    queryGetFirstSales.prepare("SELECT purschase.sum FROM purschase\
                                WHERE date >= :date::date  AND date <=  :secondDate::date\
                                AND purschase.id_status != 5");
    queryGetFirstSales.bindValue(":date", date);
    queryGetFirstSales.bindValue(":secondDate", lastDate);

    if (queryGetFirstSales.exec())
    {
        while (queryGetFirstSales.next())
        {
            sum += queryGetFirstSales.value(0).toInt();
        }
        sales.push_back(sum);
        sum = 0;
    }

    date = QString::number(secondDate.year()) + "." + QString::number(secondDate.month()) + ".01";
    lastDate = QString::number(secondDate.year()) + "." + QString::number(secondDate.month()) + "." +
                           QString::number(secondDate.daysInMonth());

    QSqlQuery queryGetSecondSales(db_);
    queryGetSecondSales.prepare("SELECT Purschase.sum FROM Purschase\
                                 WHERE date >= :date::date  AND date <=  :secondDate::date\
                                 AND Purschase.id_status != 5");
    queryGetSecondSales.bindValue(":date", date);
    queryGetSecondSales.bindValue(":secondDate", lastDate);

    if (queryGetSecondSales.exec())
    {
        while (queryGetSecondSales.next())
        {
            sum += queryGetSecondSales.value(0).toInt();
        }
        sales.push_back(sum);
        sum = 0;
    }

    date = QString::number(thirstDate.year()) + "." + QString::number(thirstDate.month()) + ".01";
    lastDate = QString::number(thirstDate.year()) + "." + QString::number(thirstDate.month()) + "." +
                           QString::number(thirstDate.daysInMonth());

    QSqlQuery queryGetThirstSales(db_);
    queryGetThirstSales.prepare("SELECT Purschase.sum FROM Purschase\
                                 WHERE date >= :date::date  AND date <=  :secondDate::date\
                                 AND Purschase.id_status != 5");
    queryGetThirstSales.bindValue(":date", date);
    queryGetThirstSales.bindValue(":secondDate", lastDate);

    if (queryGetThirstSales.exec())
    {
        while (queryGetThirstSales.next())
        {
            sum += queryGetThirstSales.value(0).toInt();
        }
        sales.push_back(sum);
    }

    return sales;
}


std::vector<std::pair<std::string, int>> jp::Logic::getTopBook()
{
    std::vector<std::pair<std::string, int>> topBook;

    QSqlQuery queryGetTopBook(db_);
    queryGetTopBook.prepare("SELECT book.title, SUM(basket.count) FROM basket\
                            JOIN book ON book.id = basket.id_book\
                            GROUP BY book.title\
                            order by SUM(basket.count) desc\
                            LIMIT 5");
    if (queryGetTopBook.exec())
    {
        while (queryGetTopBook.next())
        {
            topBook.push_back(std::make_pair(queryGetTopBook.value(0).toString().toStdString(),
                                             queryGetTopBook.value(1).toInt()));
        }
    }
    return topBook;
}


std::vector<std::pair<std::string, int>> jp::Logic::getTopBookMonth()
{
    std::vector<std::pair<std::string, int>> topBook;
    QDate firstDate = QDate::currentDate();
    QDate secondDate = firstDate.addMonths(-1);

    QString lastDate = QString::number(firstDate.year()) + "." + QString::number(firstDate.month()) + "." + QString::number(firstDate.day());
    QString date = QString::number(secondDate.year()) + "." + QString::number(secondDate.month()) + "." +
                                   QString::number(secondDate.day());

    QSqlQuery queryGetTopBook(db_);
    queryGetTopBook.prepare("SELECT book.title, SUM(basket.count) FROM basket\
                            JOIN book ON book.id = basket.id_book\
                            JOIN purschase ON purschase.id = basket.id_purschase\
                            WHERE date >= :firstDate::date AND date <= :secondDate::date AND Purschase.id_status != 5\
                            GROUP BY book.title\
                            ORDER BY SUM(Basket.count) desc\
                            LIMIT 5");
    queryGetTopBook.bindValue(":firstDate", date);
    queryGetTopBook.bindValue(":secondDate", lastDate);
    if (queryGetTopBook.exec())
    {
        while (queryGetTopBook.next())
        {
            topBook.push_back(std::make_pair(queryGetTopBook.value(0).toString().toStdString(),
                                             queryGetTopBook.value(1).toInt()));
        }
    }
    return topBook;
}


std::vector<std::pair<std::string, int>> jp::Logic::getTopClient()
{
    std::vector<std::pair<std::string, int>> topClient;

    QSqlQuery queryGetTopClient(db_);
    queryGetTopClient.prepare("SELECT client.name, COUNT(purschase.id) FROM purschase\
                              JOIN client ON client.id = purschase.id_client\
                              WHERE Purschase.id_status != 5\
                              GROUP BY client.name\
                              ORDER BY COUNT(purschase.id) desc\
                              LIMIT 10");
    if (queryGetTopClient.exec())
    {
        while (queryGetTopClient.next())
        {
            topClient.push_back(std::make_pair(queryGetTopClient.value(0).toString().toStdString(),
                                             queryGetTopClient.value(1).toInt()));
        }
    }

    return topClient;
}
