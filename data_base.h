
#ifndef DATA_BASE_H
#define DATA_BASE_H

#include <QSettings>
#include <QtSql/QSqlDatabase>

namespace jp
{
    class DataBase
    {
    public:
        DataBase();

        bool connection() noexcept(false);
        QSqlDatabase& getContext() noexcept;

    private:
        std::string dataBaseName_;
        std::string userName_;
        std::string password_;
        QSettings settings_;
        QSqlDatabase db_;
    };
}

#endif
