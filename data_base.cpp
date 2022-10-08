
#include <QApplication>
#include <QMessageBox>
#include <QSqlError>
#include "data_base.h"

jp::DataBase::DataBase():
   settings_("settings.ini", QSettings::IniFormat)
{
    dataBaseName_ = settings_.value("db_name").value<QString>().toStdString();;
    userName_     = settings_.value("user_name").value<QString>().toStdString();;
    password_     = settings_.value("password").value<QString>().toStdString();
}


bool jp::DataBase::connection() noexcept(false)
{
    if (!dataBaseName_.empty() && !userName_.empty() && !password_.empty())
    {
        db_ = QSqlDatabase::addDatabase("QPSQL");
        db_.setDatabaseName(QString::fromStdString(dataBaseName_));
        db_.setUserName(QString::fromStdString(userName_));
        db_.setPassword(QString::fromStdString(password_));
    }

    return db_.open();
}


QSqlDatabase &jp::DataBase::getContext() noexcept
{
    return db_;
}
