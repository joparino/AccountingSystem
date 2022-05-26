
#include <QMessageBox>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "controller.h"

jp::Controller::Controller(MainWindow& window, AddOrderWindow& ow, AddBookWindow& bw, AddArrivalWindow& aw, QSqlDatabase& db):
    window_(window),
    ow_(ow),
    bw_(bw),
    aw_(aw),
    db_(db),
    bf_(7),
    bfb_(7)
{}


// Запрос к бд для получения данных о заказе
void jp::Controller::loaderOrder() noexcept
{
    QSqlQuery queryOrder(db_);
    queryOrder.exec( "SELECT Client.name, STRING_AGG(Book.title, ', '), Purschase.address, Status.type, Purschase.date, Purschase.sum FROM Purschase\
                     JOIN Client ON Client.id = Purschase.id_client\
                     JOIN Status ON Status.id = Purschase.id_status\
                     JOIN Basket ON Basket.id_purschase = Purschase.id\
                     JOIN Book ON BOOK.id = Basket.id_book\
                     GROUP BY Client.name, Purschase.address, Status.type, Purschase.date, Purschase.sum");

    while (queryOrder.next())
    {
        bf_.insert(queryOrder.value(0).toString().toStdString().c_str());
        window_.addRowOrder(queryOrder.value(0).toString(), queryOrder.value(1).toString(), queryOrder.value(2).toString(),
                            queryOrder.value(3).toString(), queryOrder.value(4).toString(), queryOrder.value(5).toString());
    }
}


void jp::Controller::loaderBook() noexcept
{
    QSqlQuery queryBook(db_);
    queryBook.exec( "SELECT Book.title, Author.nameOfAuthor, Genre.nameOfGenre, Publisher.nameOfPublisher, Book.year, Book.price, Book.count FROM Book\
                    JOIN Author ON Author.id = Book.id_author\
                    JOIN Genre ON Genre.id = Book.id_genre\
                    JOIN Publisher ON Publisher.id = Book.id_publisher");

    while (queryBook.next())
    {
        bfb_.insert(queryBook.value(0).toString().toStdString().c_str());
        window_.addRowBook(queryBook.value(0).toString(), queryBook.value(1).toString(),
                           queryBook.value(2).toString(), queryBook.value(3).toString(),
                           queryBook.value(4).toString(), queryBook.value(5).toString(),
                           queryBook.value(6).toString());
    }
}


void jp::Controller::loaderOrderWindow() noexcept
{
    QSqlQuery queryTitleBook(db_);
    queryTitleBook.exec( "SELECT Book.title FROM Book");

    while (queryTitleBook.next())
    {
        ow_.addTitleBook(queryTitleBook.value(0).toString());
    }
}


void jp::Controller::loaderBookWindow() noexcept
{
    QSqlQuery queryAuthor(db_);
    QSqlQuery queryPublisher(db_);
    QSqlQuery queryGenre(db_);
    QSqlQuery queryTitle(db_);
    queryAuthor.exec( "SELECT nameOfAuthor FROM Author");
    queryPublisher.exec( "SELECT nameOfPublisher FROM Publisher");
    queryGenre.exec( "SELECT nameOfGenre FROM Genre");
    queryTitle.exec( "SELECT title FROM Book");

    while (queryAuthor.next())
    {
        bw_.addItemAuthor(queryAuthor.value(0).toString());
    }

    while (queryPublisher.next())
    {
        bw_.addItemPublisher(queryPublisher.value(0).toString());
    }

    while (queryGenre.next())
    {
        bw_.addItemGenre(queryGenre.value(0).toString());
    }

    while (queryTitle.next())
    {
        aw_.addItemBook(queryTitle.value(0).toString());
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
        QSqlQuery querySearchOrder(db_);
        querySearchOrder.prepare("SELECT Client.name, STRING_AGG(Book.title, ', '), Purschase.address, Status.type, Purschase.date, Purschase.sum FROM Purschase\
                            JOIN Client ON Client.id = Purschase.id_client\
                            JOIN Status ON Status.id = Purschase.id_status\
                            JOIN Basket ON Basket.id_purschase = Purschase.id\
                            JOIN Book ON BOOK.id = Basket.id_book\
                            WHERE Client.name LIKE :name\
                            GROUP BY Client.name, Purschase.address, Status.type, Purschase.date, Purschase.sum");
        // querySearch.bindValue(":name", QString("%") + str + '%');
        querySearchOrder.bindValue(":name", str);
        querySearchOrder.exec();

        window_.clearOrder();

        while (querySearchOrder.next())
        {
            window_.addRowOrder(querySearchOrder.value(0).toString(), querySearchOrder.value(1).toString(),
                                querySearchOrder.value(2).toString(), querySearchOrder.value(3).toString(),
                                querySearchOrder.value(4).toString(), querySearchOrder.value(5).toString());
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
        QSqlQuery querySearchBook(db_);
        querySearchBook.prepare("SELECT Book.title, Author.nameOfAuthor, STRING_AGG(Genre.nameOfGenre, ', '), Publisher.nameOfPublisher, Book.year, Book.price, Book.count FROM Book\
                                JOIN Author ON Author.id = Book.id_author\
                                JOIN Genre ON Genre.id = Book.id_genre\
                                JOIN Publisher ON Publisher.id = Book.id_publisher\
                                WHERE Book.title LIKE :title\
                                GROUP BY Book.title, Author.nameOfAuthor, Genre.nameOfGenre, Publisher.nameOfPublisher, Book.year, Book.price, Book.count");
        // querySearch.bindValue(":name", QString("%") + str + '%');
        querySearchBook.bindValue(":title", str);
        querySearchBook.exec();

        window_.clearBook();

        while (querySearchBook.next())
        {
            window_.addRowBook(querySearchBook.value(0).toString(), querySearchBook.value(1).toString(),
                               querySearchBook.value(2).toString(), querySearchBook.value(3).toString(),
                               querySearchBook.value(4).toString(), querySearchBook.value(5).toString(),
                               querySearchBook.value(6).toString());
        }
    }
    else
    {
        QMessageBox::information(0, "Не найдено", "Такой книги нет!");
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

//    if (!queryAddBook.isValid())
//    {
//        QMessageBox::critical(nullptr, "Неверно заполнены поля", "Проверьте поля и заполните их правильно");
//    }

    loaderBook();
    bw_.reject();
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

    window_.clearBook();
    loaderBook();
    aw_.reject();
}
