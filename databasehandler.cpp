#include "databasehandler.h"

DatabaseHandler::DatabaseHandler() {}

DatabaseHandler::~DatabaseHandler() {}

void DatabaseHandler::createDatabase()
{

    QString sqlCommand = "CREATE DATABASE IF NOT EXISTS mydb";


    bulkClubDatabase.exec(sqlCommand);

}

void DatabaseHandler::createTables()
{

    QString purchaseinfoTable = "CREATE TABLE IF NOT EXISTS mydb.purchaseinfo (ID INT PRIMARY KEY, PurchaseDate DATE, CustomerNumber INT, Product VARCHAR(50), "
                                "Price DOUBLE, Quantity INT)";
    QString shopperinforTable = "CREATE TABLE IF NOT EXISTS mydb.shoppersinfo (CustomerName VARCHAR(25), CustomerNumber INT PRIMARY KEY, Membership VARCHAR(15), MembershipExpireDate DATE)";

    QString inventryTable     = "CREATE TABLE IF NOT EXISTS mydb.inventory (Product VARCHAR(50) PRIMARY KEY, Price DOUBLE)";

    bulkClubDatabase.exec(purchaseinfoTable);

    bulkClubDatabase.exec(shopperinforTable);

    bulkClubDatabase.exec(inventryTable);

}

void DatabaseHandler::createSaleReportTable()
{
    QString salesReportTable = "CREATE TABLE IF NOT EXISTS mydb.salesreport (ID INT PRIMARY KEY, PurchaseDate DATE, CustomerName VARCHAR(25),  CustomerNumber INT, Product VARCHAR(50), "
                               "Price DOUBLE, Quantity INT, Membership VARCHAR(15), MembershipExpireDate DATE)";

    bulkClubDatabase.exec(salesReportTable);
}

void DatabaseHandler::loadDataIntoCombinedTable()
{

    QSqlQuery query(bulkClubDatabase);

    // Update and Fill the membership information in Sales Report table
    QString fillMembershipSalesReportQuery = "UPDATE mydb.salesreport sa, mydb.shoppersinfo sh "
                                             "SET "
                                             "sa.CustomerName = sh.CustomerName, "
                                             "sa.Membership = sh.Membership, "
                                             "sa.MembershipExpireDate = sh.MembershipExpireDate "
                                             "WHERE "
                                             "sa.CustomerNumber = sh.CustomerNumber "
                                             "AND "
                                             "ID > 0";

    query.prepare(fillMembershipSalesReportQuery);

    bool isExec = query.exec();

    if (!isExec)
    {
        qDebug() << query.lastError().text();
    }
    else
    {
        qDebug() << "Combined two tables";
    }
}


void DatabaseHandler::connectionToDatabase()
{

    bulkClubDatabase = QSqlDatabase::addDatabase("QMYSQL", "mydb");
    bulkClubDatabase.setHostName("localhost");
    bulkClubDatabase.setPort(3306);
    bulkClubDatabase.setUserName("root");
    bulkClubDatabase.setPassword("root123");
    if (!bulkClubDatabase.open())
    {
        qDebug() << bulkClubDatabase.lastError().text();
    }
    else
    {
        qDebug() << "Success!";
    }
}

bool DatabaseHandler::openDatabase()
{
    return bulkClubDatabase.open();
}

void DatabaseHandler::selectDatabase()
{
    bulkClubDatabase.close();
    bulkClubDatabase.setDatabaseName("mydb");
    bulkClubDatabase.open();
}

void DatabaseHandler::closeDatabase()
{
    bulkClubDatabase.close();
}

QString DatabaseHandler::connectionStatus()
{
    return bulkClubDatabase.lastError().text();
}


QSqlDatabase DatabaseHandler::getDatabase() const
{
    return bulkClubDatabase;
}
