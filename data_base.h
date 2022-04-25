#ifndef DATA_BASE_H
#define DATA_BASE_H

#include <QtSql/QSqlDatabase>

namespace jp
{
    class DataBase
    {
    public:
        QSqlDatabase db;
        void Connection();
    };
}

#endif // DATA_BASE_H
