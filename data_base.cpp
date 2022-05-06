
#include <QApplication>
#include <QMessageBox>
#include <QSqlError>
#include "data_base.h"

const void jp::DataBase::connection() noexcept(false)
{
    jp::DataBase::db_ = QSqlDatabase::addDatabase("QODBC3");
    QString host = "localhost";
    QString database = "BookStore";
    db_.setDatabaseName(QString("DRIVER={SQL Server};"
                "SERVER=DESKTOP-FPF4P4U\\SQLEXPRESS;DATABASE=BookStore;Persist Security Info=true;")
              .arg(host, database));

    if(db_.open())
    {
        QMessageBox::information(0, "Успех", "");
    }
    else
    {
        QMessageBox::critical(0, "Ошибка", db_.lastError().text());
    }
}


QSqlDatabase& jp::DataBase::getContext() noexcept
{
    return db_;
}
