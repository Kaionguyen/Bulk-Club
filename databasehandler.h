#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <iostream>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <memory>


class DatabaseHandler
{
    public:
        /**
          * @brief DatabaseHandler constructor and destructor
          */
         DatabaseHandler();
         ~DatabaseHandler();

         /**
          * @brief createDatabase
          */
         void createDatabase();
         void createTables();
         void createSaleReportTable();
         void loadDataIntoCombinedTable();

         /**
          * @brief building the connection to the mysql database
          */
         void connectionToDatabase();
         bool openDatabase();
         void closeDatabase();
         void selectDatabase();
         QString connectionStatus();

         /**
           * @brief create table in data base
           */
         void    createDataTable();


         QSqlDatabase getDatabase() const;



    private:
         QString      connectionName;
         QSqlDatabase bulkClubDatabase;
         QSqlQuery    *query;
};

#endif // DATABASEHANDLER_H
