
#include <QMessageBox>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <set>
#include "controller.h"
#include "qdatetime.h"
#include "qspinbox.h"

jp::Controller::Controller(MainWindow& window, OrderWindow& ow, AddClientWindow&   acw, AddOrderWindow& aow,
                           AddBookWindow& abw, AddArrivalWindow& aaw, QSqlDatabase& db):
    window_(window),
    order_window_(ow),
    add_client_window_(acw),
    add_order_window_(aow),
    add_book_window_(abw),
    add_arribal_window_(aaw),
    db_(db),
    bf_(7),
    bfb_(7)
{
    QSqlQuery queryAuthor(db_);
    queryAuthor.exec( "SELECT Author.nameOfAuthor FROM Author");

    while (queryAuthor.next())
    {
        std::shared_ptr<jp::Author> author(new jp::Author);
        author->name = queryAuthor.value(0).toString().toStdString();
        authors_.push_back(author);
    };

    QSqlQuery queryGenre(db_);
    queryGenre.exec( "SELECT Genre.nameOfGenre FROM Genre");

    while (queryGenre.next())
    {
        std::shared_ptr<jp::Genre> genre(new jp::Genre);
        genre->name = queryGenre.value(0).toString().toStdString();
        genres_.push_back(genre);
    };

    QSqlQuery queryPublisher(db_);
    queryPublisher.exec( "SELECT Publisher.nameOfPublisher FROM Publisher");

    while (queryPublisher.next())
    {
        std::shared_ptr<jp::Publisher> publisher(new jp::Publisher);
        publisher->name = queryPublisher.value(0).toString().toStdString();
        publishers_.push_back(publisher);
    };

    QSqlQuery queryClient(db_);
    queryClient.exec( "SELECT id, name, phoneNumber FROM Client");

    while (queryClient.next())
    {
        std::shared_ptr<jp::Client> client(new jp::Client);
        client->id = queryClient.value(0).toString().toInt();
        client->name = queryClient.value(1).toString().toStdString();
        client->phoneNumber = queryClient.value(2).toString().toStdString();
        clients_.push_back(client);
    };

    QSqlQuery queryStatus(db_);
    queryStatus.exec( "SELECT id, type FROM Status");

    while (queryStatus.next())
    {
        std::shared_ptr<jp::Status> status(new jp::Status);
        status->id = queryStatus.value(0).toString().toInt();
        status->name = queryStatus.value(1).toString().toStdString();
        statuses_.push_back(status);
    };

    QSqlQuery queryBook(db_);
    queryBook.exec( "SELECT Book.id, title, Author.nameOfAuthor, Genre.nameOfGenre, Publisher.nameOfPublisher, year, price, count FROM Book\
                    JOIN Author ON Author.id = Book.id_author\
                    JOIN Genre ON Genre.id = Book.id_genre\
                    JOIN Publisher ON Publisher.id = Book.id_publisher");

    while (queryBook.next())
    {
        std::shared_ptr<jp::Book> book(new jp::Book);

        Authors::iterator author       = std::find(authors_.begin(), authors_.end(),
                                                   queryBook.value(2).toString().toStdString());
        Genres::iterator genre         = std::find(genres_.begin(), genres_.end(),
                                                   queryBook.value(3).toString().toStdString());
        Publishers::iterator publisher = std::find(publishers_.begin(), publishers_.end(),
                                                   queryBook.value(4).toString().toStdString());

        book->id = queryBook.value(0).toString().toInt();
        book->title = queryBook.value(1).toString().toStdString();
        book->author = *author;
        book->genre = *genre;
        book->publisher = *publisher;
        book->year = queryBook.value(5).toString().toStdString();
        book->price = queryBook.value(6).toFloat();
        book->count = queryBook.value(7).toInt();
        books_.push_back(book);

        bfb_.insert(book->title.c_str());
        window_.addRowBook(book);
    }

    QSqlQuery queryOrder(db_);
    queryOrder.exec( "SELECT Purschase.id, Client.phoneNumber, Employee.id, STRING_AGG(Book.title, ', '), Purschase.address, Status.type, Purschase.date, Purschase.sum FROM Purschase\
                     JOIN Client ON Client.id = Purschase.id_client\
                     JOIN Employee ON Employee.id = Purschase.id_employee\
                     JOIN Status ON Status.id = Purschase.id_status\
                     JOIN Basket ON Basket.id_purschase = Purschase.id\
                     JOIN Book ON Book.id = Basket.id_book\
                     GROUP BY Purschase.id, Client.phoneNumber, Employee.id, Purschase.address, Status.type, Purschase.date, Purschase.sum");

    while (queryOrder.next())
    {

        std::shared_ptr<jp::Order> order(new jp::Order);

        Clients::iterator client     = std::find(clients_.begin(), clients_.end(),
                                                 queryOrder.value(1).toString().toStdString());
        Statuses::iterator status    = std::find(statuses_.begin(), statuses_.end(),
                                                 queryOrder.value(5).toString().toStdString());

//        std::string  books = queryOrder.value(3).toString().toStdString();
//        std::string delimiter = ", ";
//        size_t pos = 0;

//        while ((pos = books.find(delimiter)) != std::string::npos)
//        {
//             std::string token = books.substr(0, pos);
//             Books::iterator book = std::find(books_.begin(), books_.end(), token);
//             order->books.push_back(**book);
//             books.erase(1, pos + delimiter.length());
//        }

        QSqlQuery queryBasket(db_);
        queryBasket.prepare("SELECT id_book, id_purschase, count FROM Basket WHERE id_purschase = :id");
        queryBasket.bindValue(":id", queryOrder.value(0).toInt());
        queryBasket.exec();
        while (queryBasket.next())
        {
             Books::iterator book = std::find(books_.begin(), books_.end(), queryBasket.value(0).toString().toInt());
             jp::Book bookFound = **book;
             bookFound.count = queryBasket.value(2).toInt();
             order->books.push_back(bookFound);
        }

        order->id = queryOrder.value(0).toString().toInt();
        order->client = *client;
        order->status = *status;
        order->employee = 1;
        order->address = queryOrder.value(4).toString().toStdString();
        order->date = queryOrder.value(6).toString().toStdString();
        order->sum = queryOrder.value(7).toString().toInt();
        orders_.push_back(order);

        bf_.insert(order->client->name.c_str());
        window_.addRowOrder(orders_.back());
    }
}

void jp::Controller::loaderOrder() noexcept
{
   for (auto& order : orders_)
   {
       window_.addRowOrder(order);
   }
}


void jp::Controller::loaderBook() noexcept
{
    for (auto& book : books_)
    {
        window_.addRowBook(book);
    }
}


void jp::Controller::loaderOrderWindow() noexcept
{
    add_order_window_.clearBooks();
    for (auto& book : books_)
    {
        add_order_window_.addTitleBook(book->title, book);
    }

    order_window_.clearStatus();
    for (auto& status : statuses_)
    {
        order_window_.addStatus(status);
    }
}


void jp::Controller::loaderBookWindow() noexcept
{
    for (auto& author : authors_)
    {
        add_book_window_.addItemAuthor(QString::fromStdString(author->name));
    }

    for (auto& publisher : publishers_)
    {
        add_book_window_.addItemPublisher(QString::fromStdString(publisher->name));
    }

    for (auto& genre : genres_)
    {
        add_book_window_.addItemGenre(QString::fromStdString(genre->name));
    }

    for (auto& book : books_)
    {
        add_arribal_window_.addItemBook(QString::fromStdString(book->title));
    }
}


void jp::Controller::searchOrder(QString str)
{
    if (str == "")
    {
        window_.clearOrder();
        loaderOrder();
    }
    if (bf_.check(str.toStdString().c_str()))
    {
        //Orders::iterator order = std::find(orders_.begin(), orders_.end(), str.toStdString());

        window_.clearOrder();
        for (auto& order : orders_)
        {
            if (order->client->name == str.toStdString())
            {
               window_.addRowOrder(order);
            }
        }
    }
    else
    {
        QMessageBox::information(0, "Не найдено", "Такого пользователя нет!");
    }
}


void jp::Controller::searchBook(QString str)
{
    if (str == "")
    {
        window_.clearBook();
        loaderBook();
    }
    if (bfb_.check(str.toStdString().c_str()))
    {
        Books::iterator book = std::find(books_.begin(), books_.end(), str.toStdString());

        window_.clearBook();
        window_.addRowBook(*book);
    }
    else
    {
        QMessageBox::information(0, "Не найдено", "Такой книги нет!");
    }
}


void jp::Controller::addClient(std::string name, std::string number)
{
    Clients::iterator clientFound  = std::find(clients_.begin(), clients_.end(), number);
    if (clientFound._Ptr->_Myval == nullptr)
    {
        bool flag = false;
        QSqlQuery queryAddClient(db_);
        queryAddClient.prepare("INSERT Client VALUES (:name, :number)\
                                SELECT @@identity");
        queryAddClient.bindValue(":name", QString::fromStdString(name));
        queryAddClient.bindValue(":number", QString::fromStdString(number));
        queryAddClient.exec();

        QString idClient = "";
        while(queryAddClient.next())
        {
            idClient = queryAddClient.value(0).toString();
            flag = true;
        }

        if (flag == true)
        {
            jp::Client client;
            client.id = idClient.toInt();
            client.name = name;
            client.phoneNumber = number;
            clients_.push_back(std::make_shared<jp::Client>(client));
            add_client_window_.reject();
        }
    }
}


void jp::Controller::addBook(QString title, QString author, QString genre,
                             QString publisher, QString year, QString price)
{
    QSqlQuery queryAddBook(db_);
    queryAddBook.prepare("DECLARE @AuthorID int\
                        DECLARE @GenreID int\
                        DECLARE @PublisherID int\
                        set @AuthorID = (SELECT id FROM Author Where nameOfAuthor = :author)\
                        set @GenreID = (SELECT id FROM Genre Where nameOfGenre = :genre)\
                        set @PublisherID = (SELECT id FROM Publisher Where nameOfPublisher = :publisher)\
                        INSERT Book VALUES (:title, @AuthorID, @GenreID, @PublisherID, :year, :price, '0')");
    queryAddBook.bindValue(":title", title);
    queryAddBook.bindValue(":author", author);
    queryAddBook.bindValue(":genre", genre);
    queryAddBook.bindValue(":publisher", publisher);
    queryAddBook.bindValue(":year", year);
    queryAddBook.bindValue(":price", price);
    queryAddBook.exec();

    std::shared_ptr<jp::Book> book(new jp::Book);

    Authors::iterator authorFound       = std::find(authors_.begin(), authors_.end(),
                                               author.toStdString());
    Genres::iterator genreFound         = std::find(genres_.begin(), genres_.end(),
                                               genre.toStdString());
    Publishers::iterator publisherFound = std::find(publishers_.begin(), publishers_.end(),
                                               publisher.toStdString());

    book->title = title.toStdString();
    book->author = *authorFound;
    book->genre = *genreFound;
    book->publisher = *publisherFound;
    book->year = year.toStdString();
    book->price = price.toFloat();
    book->count = 0;
    books_.push_back(book);
    bfb_.insert(book->title.c_str());

    window_.clearBook();
    loaderBook();
    add_book_window_.reject();
}


void jp::Controller::addArrival(QString title, QString count)
{
    QSqlQuery queryAddArrival(db_);
    queryAddArrival.prepare("DECLARE @BookID int\
                            set @BookID = (SELECT id FROM Book Where title = :title)\
                            UPDATE Book SET count = count + :count WHERE id = @BookID");
    queryAddArrival.bindValue(":title", title);
    queryAddArrival.bindValue(":count", count);
    queryAddArrival.exec();

    Books::iterator titleFound       = std::find(books_.begin(), books_.end(),
                                               title.toStdString());
    titleFound._Ptr->_Myval->count = titleFound._Ptr->_Myval->count + count.toInt();

    window_.clearBook();
    loaderBook();
    add_arribal_window_.reject();
}


void jp::Controller::addOrder(QString number, QString address, QString sum, QTableWidget* comboBook)
{
    QSqlQuery queryAddPurchase(db_);
    queryAddPurchase.prepare("DECLARE @ClientID int\
                             set @ClientID = (SELECT id FROM Client Where phoneNumber = :number)\
                             INSERT Purschase VALUES (1, @ClientID, 1, :address, :date, :sum)\
                             SELECT @@identity");
    queryAddPurchase.bindValue(":number", number);
    queryAddPurchase.bindValue(":address", address);
    queryAddPurchase.bindValue(":date", QDate::currentDate().toString("dd.MM.yyyy"));
    queryAddPurchase.bindValue(":sum", sum);
    queryAddPurchase.exec();

    QString idPurchase = "";
    while(queryAddPurchase.next())
    {
        idPurchase = queryAddPurchase.value(0).toString();
    }

    Clients::iterator client  = std::find(clients_.begin(), clients_.end(), number.toStdString());
    Statuses::iterator status = std::find(statuses_.begin(), statuses_.end(), 4);

    std::shared_ptr<jp::Order> order(new jp::Order);

    order->id = idPurchase.toInt();
    order->client = *client;
    order->status = *status;
    order->employee = 1;
    order->address = address.toStdString();
    order->date = QDate::currentDate().toString("dd.MM.yyyy").toStdString();
    order->sum = sum.toFloat();

    for (size_t i = 0; i < comboBook->rowCount(); ++i)
    {
        QSpinBox* count = qobject_cast<QSpinBox*>(comboBook->cellWidget(i, 2));

        QSqlQuery queryAddInBasket(db_);
        queryAddInBasket.prepare("DECLARE @BookID int\
                                 set @BookID = (SELECT id FROM Book Where title = :title)\
                                 INSERT Basket VALUES (@BookID, :idPurchase, :count)");
        queryAddInBasket.bindValue(":title", comboBook->item(i, 0)->text());
        queryAddInBasket.bindValue(":count", count->text());
        queryAddInBasket.bindValue(":idPurchase", idPurchase);
        queryAddInBasket.exec();

        QSqlQuery queryCountBook(db_);
        queryCountBook.prepare("DECLARE @BookID int\
                                set @BookID = (SELECT id FROM Book Where title = :title)\
                                UPDATE Book SET count = count - :count WHERE id = @BookID");
        queryCountBook.bindValue(":title", comboBook->item(i, 0)->text());
        queryCountBook.bindValue(":count", count->text());
        queryCountBook.exec();

        Books::iterator book = std::find(books_.begin(), books_.end(), comboBook->item(i, 0)->text().toStdString());
        jp::Book bookFound = **book;
        bookFound.count = count->text().toInt();
        order->books.push_back(bookFound);
    }

    orders_.push_back(order);
    bf_.insert(order->client->name.c_str());

    window_.clearOrder();
    loaderOrder();
}


void jp::Controller::hasOrderChanged(std::shared_ptr<jp::Order> order, int status)
{
    Statuses::iterator statusFound  = std::find(statuses_.begin(), statuses_.end(), status);
    order->status = *statusFound;

    QSqlQuery queryChangeClient(db_);
    queryChangeClient.prepare("UPDATE Client SET name = :name, phoneNumber = :number WHERE id = :id_client");
    queryChangeClient.bindValue(":id_client", order->client->id);
    queryChangeClient.bindValue(":name", QString::fromStdString(order->client->name));
    queryChangeClient.bindValue(":number", QString::fromStdString(order->client->phoneNumber));
    queryChangeClient.exec();

    QSqlQuery queryChangeOrder(db_);
    queryChangeOrder.prepare("UPDATE Purschase SET address = :address, id_status = :status WHERE id = :id_order");
    queryChangeOrder.bindValue(":id_order", order->id);
    queryChangeOrder.bindValue(":address", QString::fromStdString(order->address));
    queryChangeOrder.bindValue(":status", order->status->id);
    queryChangeOrder.exec();

    window_.clearOrder();
    loaderOrder();
}


void jp::Controller::authorization(std::string login, std::string password)
{
    QSqlQuery queryAuthorization(db_);
    queryAuthorization.prepare("SELECT role, password FROM Employee\
                                WHERE login = :login");
    queryAuthorization.bindValue(":login", QString::fromStdString(login));
    queryAuthorization.exec();


    while(queryAuthorization.next())
    {
        if (password == queryAuthorization.value(1).toString().toStdString())
        {
            if (queryAuthorization.value(1).toString().toInt() == 0)
            {
                loaderOrderWindow();
                loaderBookWindow();
                window_.show();
            }
            else
            {
                int g = 5;
            }
        }
        else
        {
            QMessageBox::critical(nullptr, "Ошибка", "Неправильный логин или пароль");
        }
    }


}
