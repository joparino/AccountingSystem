#include "data_layer.h"
#include "qsqlquery.h"
#include "qvariant.h"

jp::DataLayer::DataLayer(QSqlDatabase& db):
    db_(db),
    bf_(7),
    bfb_(7)
{
    QSqlQuery queryAuthor(db_);
    queryAuthor.exec( "SELECT * FROM author");

    while (queryAuthor.next())
    {
        std::shared_ptr<jp::Author> author(new jp::Author);
        author->id = queryAuthor.value(0).toInt();
        author->name = queryAuthor.value(1).toString().toStdString();
        authors_.push_back(author);
    };

    QSqlQuery queryGenre(db_);
    queryGenre.exec( "SELECT * FROM genre");

    while (queryGenre.next())
    {
        std::shared_ptr<jp::Genre> genre(new jp::Genre);
        genre->id = queryGenre.value(0).toInt();
        genre->name = queryGenre.value(1).toString().toStdString();
        genres_.push_back(genre);
    };

    QSqlQuery queryPublisher(db_);
    queryPublisher.exec( "SELECT * FROM publisher");

    while (queryPublisher.next())
    {
        std::shared_ptr<jp::Publisher> publisher(new jp::Publisher);
        publisher->id = queryPublisher.value(0).toInt();
        publisher->name = queryPublisher.value(1).toString().toStdString();
        publishers_.push_back(publisher);
    };

    QSqlQuery queryClient(db_);
    queryClient.exec( "SELECT id, name, phone_number FROM client");

    while (queryClient.next())
    {
        std::shared_ptr<jp::Client> client(new jp::Client);
        client->id = queryClient.value(0).toString().toInt();
        client->name = queryClient.value(1).toString().toStdString();
        client->phoneNumber = queryClient.value(2).toString().toStdString();
        clients_.push_back(client);
    };


    QSqlQuery queryEmployee(db_);
    queryEmployee.exec( "SELECT * FROM employee");

    while (queryEmployee.next())
    {
        std::shared_ptr<jp::Employee> employee(new jp::Employee);
        employee->id = queryEmployee.value(0).toString().toInt();
        employee->firstName = queryEmployee.value(1).toString().toStdString();
        employee->surname = queryEmployee.value(2).toString().toStdString();
        employee->patronymic = queryEmployee.value(3).toString().toStdString();
        employee->login = queryEmployee.value(4).toString().toStdString();
        employee->password = queryEmployee.value(5).toString().toStdString();
        employee->role = queryEmployee.value(6).toInt();
        employee->isActive = queryEmployee.value(7).toBool();
        employees_.push_back(employee);
    };


    QSqlQuery queryStatus(db_);
    queryStatus.exec( "SELECT id, type FROM status");

    while (queryStatus.next())
    {
        std::shared_ptr<jp::Status> status(new jp::Status);
        status->id = queryStatus.value(0).toString().toInt();
        status->name = queryStatus.value(1).toString().toStdString();
        statuses_.push_back(status);
    };

    QSqlQuery queryBook(db_);
    queryBook.exec( "SELECT book.id, title, author.name_of_author, genre.name_of_genre, publisher.name_of_publisher, year, price, count FROM book\
                    JOIN author ON author.id = Book.id_author\
                    JOIN genre ON genre.id = Book.id_genre\
                    JOIN publisher ON publisher.id = Book.id_publisher");

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

        QSqlQuery queryBasket(db_);
        queryBasket.prepare("SELECT id_book, salePrice, count FROM Basket WHERE id_purschase = :id");
        queryBasket.bindValue(":id", queryOrder.value(0).toInt());
        queryBasket.exec();
        while (queryBasket.next())
        {
             Books::iterator book = std::find(books_.begin(), books_.end(), queryBasket.value(0).toString().toInt());
             jp::Book bookFound = **book;
             bookFound.count = queryBasket.value(2).toInt();
             bookFound.price = queryBasket.value(1).toInt();
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
    }
}
