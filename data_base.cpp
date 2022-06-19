
#include <QApplication>
#include <QMessageBox>
#include <QSqlError>
#include "data_base.h"

const void jp::DataBase::connection() noexcept(false)
{
    db_ = QSqlDatabase::addDatabase("QPSQL");
    db_.setDatabaseName("BookStore");
    db_.setUserName("postgres");
    db_.setPassword("secret");

    if (!db_.open())
    {

        QMessageBox::critical(0, "Ошибка", db_.lastError().text());
    }
}


QSqlDatabase& jp::DataBase::getContext() noexcept
{
    return db_;
}
