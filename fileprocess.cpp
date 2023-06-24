#include "fileprocess.h"

FileProcess::FileProcess()
{

}


FileProcess::~FileProcess()
{
    database.close();

}


void FileProcess::readFiles()
{

    database = QSqlDatabase::database("mydb");

    bool isOpen = database.open();

    if (!isOpen)
    {
        qDebug() << database.lastError().text();
    }

    int     id;
    QString purchaseDate;
    QString customerName;
    int     customerNumber;
    QString membership;
    QString membershipExpireDate;
    QString product;
    double  price;
    int     quantity;

    id = 1;

    // Read membership information and load into mysql

    QString shoppersInfo = ":/db/shoppers.txt";

    QFile shoppersinfoFile(shoppersInfo);

    bool shopperinfoFileOpened = shoppersinfoFile.open(QIODevice::ReadOnly);

    if (!shopperinfoFileOpened)
    {
        qDebug() << "can not open the shoppersinfo file!";
        return;
    }

    QTextStream shoppersinfoStream(&shoppersinfoFile);

    QString line = shoppersinfoStream.readLine();

    while (!shoppersinfoStream.atEnd())
    {
        customerName = line;
        shoppersinfoStream >> customerNumber;
        shoppersinfoStream.flush();
        shoppersinfoStream.readLine();
        line = shoppersinfoStream.readLine();
        membership = line;
        line = shoppersinfoStream.readLine();
        membershipExpireDate = line;

        QSqlQuery query(database);

        QString shoppersinfoQuery = QString("INSERT INTO shoppersinfo (CustomerName, CustomerNumber, Membership, MembershipExpireDate) "
                                            "VALUES ('%1', '%2', '%3', STR_TO_DATE('%4', '%m/%d/%Y')) "
                                            "ON DUPLICATE KEY UPDATE "
                                            "CustomerNumber = '%2';").arg(customerName).arg(customerNumber).arg(membership).arg(membershipExpireDate);

        query.prepare(shoppersinfoQuery);

        bool isExec = query.exec();

        if (!isExec)
        {
            qDebug() << query.lastError().text();
        }

        line = shoppersinfoStream.readLine();
    }


    // Read purchase information and load into mysql
    for (int i = 1; i <= 7; i++)
    {

        QString purchaseInfo = ":/db/day" + QString::number(i) + ".txt";

        QFile purchaseinfoFile(purchaseInfo);

        bool purchaseinfoFileOpened = purchaseinfoFile.open(QIODevice::ReadOnly);

        if (!purchaseinfoFileOpened)
        {
            qDebug() << "can not open the purchaseinfo file!";
            return;
        }

        QTextStream purchaseinfoStream(&purchaseinfoFile);

        QString line = purchaseinfoStream.readLine();
        // Read purchase information for 7 days and load into mysql
        while (!purchaseinfoStream.atEnd())
        {

            purchaseDate = line;
            purchaseinfoStream >> customerNumber;
            purchaseinfoStream.flush();
            purchaseinfoStream.readLine();
            line = purchaseinfoStream.readLine();
            product = line;
            purchaseinfoStream >> price;
            purchaseinfoStream >> quantity;
            purchaseinfoStream.flush();

            // Insert data into purchaseinfo table
            QString purchaseinfoQuery = QString("INSERT INTO purchaseinfo (ID, PurchaseDate, CustomerNumber, Product, Price, Quantity) "
                                                "VALUES ('%1', STR_TO_DATE('%2', '%m/%d/%Y'), '%3', '%4', '%5', '%6') "
                                                "ON DUPLICATE KEY UPDATE "
                                                "ID = '%1', "
                                                "CustomerNumber = '%3';").arg(id).arg(purchaseDate).arg(customerNumber).arg(product).arg(price).arg(quantity);

            // Input data into Sales report table
            QString salesReportQuery = QString("INSERT INTO salesreport (ID, PurchaseDate, CustomerNumber, Product, Price, Quantity) "
                                               "VALUES ('%1', STR_TO_DATE('%2', '%m/%d/%Y'), '%3', '%4', '%5', '%6') "
                                               "ON DUPLICATE KEY UPDATE "
                                               "ID = '%1';").arg(id).arg(purchaseDate).arg(customerNumber).arg(product).arg(price).arg(quantity);

            // Insert data into inventory table
            QString inventoryQuery = QString("INSERT INTO inventory (Product, Price) VALUES ('%1', '%2') "
                                             "ON DUPLICATE KEY UPDATE "
                                             "Product = '%1' ;").arg(product).arg(price);

            database.exec(purchaseinfoQuery);
            database.exec(salesReportQuery);
            database.exec(inventoryQuery);

            purchaseinfoStream.readLine();
            line = purchaseinfoStream.readLine();
            id += 1;
        }

        purchaseinfoFile.close();
    }

    shoppersinfoFile.close();

}
