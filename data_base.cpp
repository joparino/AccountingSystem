
#include <QApplication>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include "data_base.h"

void jp::DataBase::Connection()
{
    jp::DataBase::db = QSqlDatabase::addDatabase("QODBC3");
    QString host = "localhost";
    QString database = "BookStore";
    db.setDatabaseName(QString("DRIVER={SQL Server};"
                "SERVER=DESKTOP-FPF4P4U\\SQLEXPRESS;DATABASE=BookStore;Persist Security Info=true;")
              .arg(host, database));

    if(db.open())
    {
        QMessageBox::information(0, "Успех", "");
    }
    else
    {
        QMessageBox::critical(0, "Ошибка", db.lastError().text());
    }

    //QSqlQuery query(db);
    //query.exec( "SELECT * FROM Client");
    //while (query.next())
    //{
    //    QString name = query.value(1).toString();
    //    QString adress = query.value(2).toString();
    //    QString phoneNumber = query.value(3).toString();
    //    bool isSupplier = query.value(4).toBool();
    //}
}
