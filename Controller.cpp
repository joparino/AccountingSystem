
#include <QMessageBox>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "controller.h"

jp::Controller::Controller(MainWindow& window, QSqlDatabase& db):
    window_(window),
    db_(db),
    bf_(7)
{}


// Запрос к бд для получения данных о заказе
void jp::Controller::loader() noexcept
{
    QSqlQuery queryOrder(db_);
    queryOrder.exec( "SELECT Client.name, Book.name, Purchase.adress, Status.type, Purchase.date, Purchase.sum FROM Purchase\
                     JOIN Client on Purchase.id_client = Client.id\
                     JOIN Book on Purchase.id_book = Book.id\
                     JOIN Status on Purchase.id_status = Status.id");

    while (queryOrder.next())
    {
        bf_.insert(queryOrder.value(0).toString().toStdString().c_str());
        window_.addRowOrder(queryOrder.value(0).toString(), queryOrder.value(1).toString(), queryOrder.value(2).toString(),
                            queryOrder.value(3).toString(), queryOrder.value(4).toString(), queryOrder.value(5).toString());
    }

    QSqlQuery queryBook(db_);
    queryBook.exec( "SELECT Book.name, Author.name, Genre.name, Book.year, Book.price, Book.count FROM Book\
                    JOIN Author ON Author.id = Book.id_author\
                    JOIN Genre ON Genre.id = Book.id_genre");

    while (queryBook.next())
    {
        window_.addRowBook(queryBook.value(0).toString(), queryBook.value(1).toString(), queryBook.value(2).toString(),
                           queryBook.value(3).toString(), queryBook.value(4).toString(), queryBook.value(5).toString());
    }
}


void jp::Controller::searchOrder(QString str)
{

    if (bf_.check(str.toStdString().c_str()))
    {
        QSqlQuery querySearch(db_);
        querySearch.prepare("SELECT Client.name, Book.name, Purchase.adress, Status.type, Purchase.date, Purchase.sum FROM Purchase\
                         JOIN Client on Purchase.id_client = Client.id\
                         JOIN Book on Purchase.id_book = Book.id\
                         JOIN Status on Purchase.id_status = Status.id\
                         WHERE Client.name LIKE :name");
        // querySearch.bindValue(":name", QString("%") + str + '%');
        querySearch.bindValue(":name", str);
        querySearch.exec();

        window_.clearOrder();

        while (querySearch.next())
        {
            window_.addRowOrder(querySearch.value(0).toString(), querySearch.value(1).toString(), querySearch.value(2).toString(),
                                querySearch.value(3).toString(), querySearch.value(4).toString(), querySearch.value(5).toString());
        }
    }
    else
    {
        QMessageBox::information(0, "Не найдено", "Такого пользователя нет!");
    }
}
