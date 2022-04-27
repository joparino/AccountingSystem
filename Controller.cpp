
#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include "controller.h"

jp::Controller::Controller(MainWindow& window, QSqlDatabase& db):
    window_(window),
    db_(db)
{}


// Запрос к бд для получения данных о заказе
void jp::Controller::LoadderOrder()
{
    QSqlQuery query(db_);
    query.exec( "SELECT * FROM Order");

    while (query.next())
    {
        window_.AddRowOrder(query.value(1).toString());
    }
}
