#ifndef FILEPROCESS_H
#define FILEPROCESS_H

#include <QFile>
#include <QDate>
#include <QDebug>

#include <string>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QMessageBox>


class FileProcess
{
    public:
        FileProcess();
        ~FileProcess();

        void readFiles();

    private:
        QFile purchaseinfoFile;
        QFile shoppersinfoFile;

        QSqlDatabase database;

};

#endif // FILEPROCES_H
