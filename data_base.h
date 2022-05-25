
#ifndef DATA_BASE_H
#define DATA_BASE_H

#include <QtSql/QSqlDatabase>

namespace jp
{
    class DataBase
    {
    public:
        const void connection() noexcept(false);
        QSqlDatabase& getContext() noexcept;

    private:
        QSqlDatabase db_;
    };
}

#endif
