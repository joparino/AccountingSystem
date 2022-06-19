
#include <QMessageBox>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <set>
#include <fstream>
#include "controller.h"
#include "qdatetime.h"
#include "qspinbox.h"
#include "QAxObject"
#include "QAxWidget"

jp::Controller::Controller(QSqlDatabase& db):
    db_(db),
    data_layer_(db_)
{}


int jp::Controller::authorization(std::string login, std::string password)
{
    QSqlQuery queryAuthorization(db_);
    queryAuthorization.prepare("SELECT role, password FROM employee\
                                WHERE login = :login");
    queryAuthorization.bindValue(":login", QString::fromStdString(login));
    queryAuthorization.exec();

    DataLayer::Employees::iterator employee = std::find(data_layer_.employees_.begin(), data_layer_.employees_.end(),
                                                        login);

    while(queryAuthorization.next())
    {
        if (password == queryAuthorization.value(1).toString().toStdString())
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


jp::DataLayer::Orders jp::Controller::searchOrder(QString str)
{
    jp::DataLayer::Orders orders;
    if (data_layer_.bf_.check(str.toStdString().c_str()))
    {
        for (auto& order : data_layer_.orders_)
        {
            if (order->client->name == str.toStdString())
            {
               orders.push_back(order);
            }
        }
    }
    return orders;
}


jp::DataLayer::Books jp::Controller::searchBook(QString str)
{
    jp::DataLayer::Books books;
    if (data_layer_.bfb_.check(str.toStdString().c_str()))
    {
        for (auto& book : data_layer_.books_)
        {
            if (book->title == str.toStdString())
            {
               books.push_back(book);
            }
        }
    }
    return books;
}


bool jp::Controller::addBook(std::shared_ptr<jp::Book> book)
{
    QSqlQuery queryAddBook(db_);
    queryAddBook.prepare("INSERT INTO book (title, id_author, id_genre, id_publisher, year, price, count)\
                          VALUES (:title, :id_author, :id_genre, id_publisher, :year, :price, '0')");
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


bool jp::Controller::addOrder(QString number, QString address, QString sum, QTableWidget* tableBook)
{
    DataLayer::Clients::iterator clientFound  = std::find(data_layer_.clients_.begin(), data_layer_.clients_.end(),
                                                          number.toStdString());
    if (clientFound != data_layer_.clients_.end())
    {
        QSqlQuery queryAddPurchase(db_);
        queryAddPurchase.prepare("INSERT INTO purschase (id_employee, id_client, id_status, address, date, sum)\
                                 VALUES (:id_employee, :id_client, 4, :address, :date, :sum)\
                                 RETURNING id");
        queryAddPurchase.bindValue(":id_employee", authorized_employee_->id);
        queryAddPurchase.bindValue(":id_client", (*clientFound)->id);
        queryAddPurchase.bindValue(":address", address);
        queryAddPurchase.bindValue(":date", QDate::currentDate().toString("dd.MM.yyyy"));
        queryAddPurchase.bindValue(":sum", sum);

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
            order->address = address.toStdString();
            order->date = QDate::currentDate().toString("dd.MM.yyyy").toStdString();
            order->sum = sum.toFloat();

            for (size_t i = 0; i < tableBook->rowCount(); ++i)
            {
                QSpinBox* count = qobject_cast<QSpinBox*>(tableBook->cellWidget(i, 2));
                std::shared_ptr<jp::Book> book = tableBook->item(i, 0)->data(Qt::UserRole).value<std::shared_ptr<jp::Book>>();


                QSqlQuery queryAddInBasket(db_);
                queryAddInBasket.prepare("INSERT INTO Basket (id_book, id_purschase, count, sale_price)\
                                          VALUES (:id, :id_purchase, :count, :price)");
                queryAddInBasket.bindValue(":id", book->id);
                queryAddInBasket.bindValue(":title", QString::fromStdString(book->title));
                queryAddInBasket.bindValue(":count", count->text());
                queryAddInBasket.bindValue(":id_purchase", idPurchase);
                queryAddInBasket.bindValue(":price", tableBook->item(i, 1)->text());
                if (!queryAddInBasket.exec())
                {
                    return false;
                }

                QSqlQuery queryCountBook(db_);
                queryCountBook.prepare("UPDATE Book SET count = count - :count WHERE id = :id");
                queryCountBook.bindValue(":id", book->id);
                queryCountBook.bindValue(":title", tableBook->item(i, 0)->text());
                queryCountBook.bindValue(":count", count->text());
                if (!queryCountBook.exec())
                {
                    return false;
                }

                jp::Book bookFound = *book;
                bookFound.count = count->text().toInt();
                book->count -= count->text().toInt();
                order->books.push_back(bookFound);
            }

            data_layer_.orders_.push_back(order);
            data_layer_.bf_.insert(order->client->name.c_str());

            return true;
        }
    }
    return false;
}


bool jp::Controller::addEmployee(std::shared_ptr<jp::Employee> employee)
{
    QSqlQuery queryAddEmployee(db_);
    queryAddEmployee.prepare("INSERT INTO employee (first_name, surname, patronymic, login, password, role, is_active)\
                              VALUES (:firstName, :surname, :patronymic,\
                                      :login, :password, :role, :isActive)");
    queryAddEmployee.bindValue(":firstName", QString::fromStdString(employee->firstName));
    queryAddEmployee.bindValue(":surname", QString::fromStdString(employee->surname));
    queryAddEmployee.bindValue(":patronymic", QString::fromStdString(employee->patronymic));
    queryAddEmployee.bindValue(":login", QString::fromStdString(employee->login));
    queryAddEmployee.bindValue(":password", QString::fromStdString(employee->password));
    queryAddEmployee.bindValue(":role", QString::number(employee->role));
    queryAddEmployee.bindValue(":isActive", QString::number(employee->isActive));

    if (queryAddEmployee.exec())
    {
        employee->id = data_layer_.employees_.back()->id + 1;
        data_layer_.employees_.push_back(employee);

        return true;
    }
    return false;
}


bool jp::Controller::addClient(std::string name, std::string number)
{
    DataLayer::Clients::iterator clientFound  = std::find(data_layer_.clients_.begin(), data_layer_.clients_.end(),
                                                          number);
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


bool jp::Controller::addArrival(std::shared_ptr<jp::Book> book, QString count)
{
    QSqlQuery queryAddArrival(db_);
    queryAddArrival.prepare("UPDATE book SET count = count + :count WHERE id = :id");
    queryAddArrival.bindValue(":id", book->id);
    queryAddArrival.bindValue(":count", count);

    if (queryAddArrival.exec())
    {
        book->count += count.toInt();
        return true;
    }
    return false;
}


bool jp::Controller::addPublisher(std::shared_ptr<jp::Publisher> publisher)
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


bool jp::Controller::addAuthor(std::shared_ptr<jp::Author> author)
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


bool jp::Controller::addGenre(std::shared_ptr<jp::Genre> genre)
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


bool jp::Controller::changeBook(std::shared_ptr<jp::Book> book)
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


bool jp::Controller::changeOrder(std::shared_ptr<jp::Order> order, int status)
{
    DataLayer::Statuses::iterator statusFound  = std::find(data_layer_.statuses_.begin(), data_layer_.statuses_.end(),
                                                           status);
    order->status = *statusFound;

    QSqlQuery queryChangeClient(db_);
    queryChangeClient.prepare("UPDATE сlient SET name = :name, phoneNumber = :number WHERE id = :id_client");
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


bool jp::Controller::changeEmployee(std::shared_ptr<jp::Employee> employee)
{
    QSqlQuery queryChangeEmployee(db_);
    queryChangeEmployee.prepare("UPDATE employee SET firstName = :firstName, surname = :surname, patronymic = :patronymic,\
                                 login = :login, password = :password, role = :role, isActive = :isActive WHERE id = :id");
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


bool jp::Controller::changePublisher(std::shared_ptr<jp::Publisher> publisher)
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


bool jp::Controller::changeAuthor(std::shared_ptr<jp::Author> author)
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


bool jp::Controller::changeGenre(std::shared_ptr<jp::Genre> genre)
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


std::vector<float> jp::Controller::getSales()
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
        sum = 0;
    }
    return sales;
}


std::vector<std::pair<std::string, int>> jp::Controller::getTopBook()
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


std::vector<std::pair<std::string, int>> jp::Controller::getTopBookMonth()
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


std::vector<std::pair<std::string, int>> jp::Controller::getTopClient()
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


bool jp::Controller::reportSales(std::string& path)
{
    std::vector<float> data = getSales();

    int firstMonth =  QDate::currentDate().month() - 2;
    int secondMonth =  QDate::currentDate().month() - 1;
    int thirstMonth =  QDate::currentDate().month();

    QString file = QString::fromStdString(path);

    QAxObject* excel = new QAxObject("Excel.Application", nullptr);
    excel->dynamicCall("SetBisible(bool)", false);
    QAxObject* workbooks = excel->querySubObject("WorkBooks");
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", file);
    QAxObject* sheets = workbook->querySubObject("Worksheets");

    QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

    QAxObject* cell = sheet->querySubObject("Cells(int, int)", 1, 1);
    cell->dynamicCall("SetValue(const QVariant&)", "Отчет продаж за 3 месяца");

    QAxObject* text1 = sheet->querySubObject("Cells(int, int)", 2, 1);
    text1->dynamicCall("SetValue(const QVariant&)", QDate::longMonthName(firstMonth));

    QAxObject* text2 = sheet->querySubObject("Cells(int, int)", 3, 1);
    text2->dynamicCall("SetValue(const QVariant&)", QDate::longMonthName(secondMonth));

    QAxObject* text3 = sheet->querySubObject("Cells(int, int)", 4, 1);
    text3->dynamicCall("SetValue(const QVariant&)", QDate::longMonthName(thirstMonth));

    QAxObject* data1 = sheet->querySubObject("Cells(int, int)", 2, 2);
    data1->dynamicCall("SetValue(const QVariant&)", data.at(0));

    QAxObject* data2 = sheet->querySubObject("Cells(int, int)", 3, 2);
    data2->dynamicCall("SetValue(const QVariant&)", data.at(1));

    QAxObject* data3 = sheet->querySubObject("Cells(int, int)", 4, 2);
    data3->dynamicCall("SetValue(const QVariant&)", data.at(2));

    workbooks->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    delete data3;
    delete data2;
    delete data1;
    delete text3;
    delete text2;
    delete text1;
    delete cell;
    delete sheet;
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;

    return true;
}


bool jp::Controller::reportSellingBooks(std::string& path)
{
    std::vector<std::pair<std::string, int>> topBook = getTopBook();

    QString file = QString::fromStdString(path);

    QAxObject* excel = new QAxObject("Excel.Application", nullptr);
    excel->dynamicCall("SetBisible(bool)", false);
    QAxObject* workbooks = excel->querySubObject("WorkBooks");
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", file);
    QAxObject* sheets = workbook->querySubObject("Worksheets");

    QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

    QAxObject* cell = sheet->querySubObject("Cells(int, int)", 1, 1);
    cell->dynamicCall("SetValue(const QVariant&)", "Самые продаваемые книги");

    for (int i = 0; i < topBook.size(); ++i)
    {
        QAxObject* text1 = sheet->querySubObject("Cells(int, int)", i + 2, 1);
        text1->dynamicCall("SetValue(const QVariant&)", QString::fromStdString(topBook.at(i).first));

        QAxObject* text2 = sheet->querySubObject("Cells(int, int)", i + 2, 2);
        text2->dynamicCall("SetValue(const QVariant&)", topBook.at(i).second);
    }

    workbooks->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    delete cell;
    delete sheet;
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;

    return true;
}


bool jp::Controller::reportSellingBooksMonth(std::string& path)
{
    std::vector<std::pair<std::string, int>> topBook = getTopBookMonth();

    QString file = QString::fromStdString(path);

    QAxObject* excel = new QAxObject("Excel.Application", nullptr);
    excel->dynamicCall("SetBisible(bool)", false);
    QAxObject* workbooks = excel->querySubObject("WorkBooks");
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", file);
    QAxObject* sheets = workbook->querySubObject("Worksheets");

    QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

    QAxObject* cell = sheet->querySubObject("Cells(int, int)", 1, 1);
    cell->dynamicCall("SetValue(const QVariant&)", "Самые продаваемые книги за месяц");

    for (int i = 0; i < topBook.size(); ++i)
    {
        QAxObject* text1 = sheet->querySubObject("Cells(int, int)", i + 2, 1);
        text1->dynamicCall("SetValue(const QVariant&)", QString::fromStdString(topBook.at(i).first));

        QAxObject* text2 = sheet->querySubObject("Cells(int, int)", i + 2, 2);
        text2->dynamicCall("SetValue(const QVariant&)", topBook.at(i).second);
    }

    workbooks->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    delete cell;
    delete sheet;
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;

    return true;
}


bool jp::Controller::reportClient(std::string& path)
{
    std::vector<std::pair<std::string, int>> topBook = getTopClient();

    QString file = QString::fromStdString(path);

    QAxObject* excel = new QAxObject("Excel.Application", nullptr);
    excel->dynamicCall("SetBisible(bool)", false);
    QAxObject* workbooks = excel->querySubObject("WorkBooks");
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", file);
    QAxObject* sheets = workbook->querySubObject("Worksheets");

    QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

    QAxObject* cell = sheet->querySubObject("Cells(int, int)", 1, 1);
    cell->dynamicCall("SetValue(const QVariant&)", "Самые частые покупатели");

    for (int i = 0; i < topBook.size(); ++i)
    {
        QAxObject* text1 = sheet->querySubObject("Cells(int, int)", i + 2, 1);
        text1->dynamicCall("SetValue(const QVariant&)", QString::fromStdString(topBook.at(i).first));

        QAxObject* text2 = sheet->querySubObject("Cells(int, int)", i + 2, 2);
        text2->dynamicCall("SetValue(const QVariant&)", topBook.at(i).second);
    }

    workbooks->dynamicCall("Close()");
    excel->dynamicCall("Quit()");

    delete cell;
    delete sheet;
    delete sheets;
    delete workbook;
    delete workbooks;
    delete excel;

    return true;
}
