#ifndef INVENTORY_H
#define INVENTORY_H

#include <QDialog>
#include <QString>
#include <QGroupBox>
#include <QGridLayout>
#include <QDebug>
#include <QTableView>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>

#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QModelIndexList>
#include <QModelIndex>

#include "addnewitem.h"

namespace Ui {
    class Inventory;
}

class Inventory : public QDialog
{
        Q_OBJECT

    public:
        explicit Inventory(QWidget *parent = nullptr);
        ~Inventory();

    private:
        Ui::Inventory *ui;

        QSqlDatabase database;
        QSqlRelationalTableModel *model;

        QTableView *inventoryTableView;

        QGroupBox *inventoryTable();
        QGroupBox *buttonBox();

        void createView();

        void addNewItem();
        void deleteItem();

};

#endif // INVENTORY_H
