#ifndef ODBC_H
#define ODBC_H



#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class ODBC
{
public:
    static bool connectToDB();
};

#endif // ODBC_H
