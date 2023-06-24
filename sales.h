#ifndef SALES_H
#define SALES_H

#include <QDialog>
#include <QLabel>
#include <QDebug>
#include <QHeaderView>
#include <QTableView>
#include <QGroupBox>
#include <QBoxLayout>
#include <QComboBox>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QLineEdit>
#include <QPushButton>

#include <QModelIndex>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>

#include "databasehandler.h"
#include "addnewsale.h"

namespace Ui {
    class Sales;
}

class Sales : public QDialog
{
        Q_OBJECT

    public:
        explicit Sales(QWidget *parent = nullptr);
        ~Sales();

    private slots:
        void purchaseDateFilter(int row);
        void customerNameFilter(int row);
        void customerNumberFilter(int row);
        void productFilter(int row);
        void membershipFilter(int row);
        void expireDateFilter(int row);

    private:
        Ui::Sales *ui;

        double subTotal;
        double executiveTotal;
        double taxTotal;
        double totalRevenue;
        double renewCost;
        double rebate;


        QLineEdit *subTotalLabel;
        QLineEdit *taxLabel;
        QLineEdit *totalRevLabel;
        QLineEdit *renewCostEdit;
        QLineEdit *rebateLabel;
        QLineEdit *recommendation;

        QComboBox *purchaseDate;
        QComboBox *customerName;
        QComboBox *customerNumber;
        QComboBox *product;
        QComboBox *membership;
        QComboBox *membershipExpireDate;

        QGroupBox *createTableViewGroupBox();
        QGroupBox *createFilterGroupBox();
        QGroupBox *revenueInfoGroupBox();
        QGroupBox *buttonBox();

        QSqlTableModel *tableModel;
        QSqlDatabase database;

        QTableView *tableView;

        QComboBox *setUpComboBox(QComboBox *box, int col, QString emptySelection);

//        // Sale report view
//        QGridLayout *layout;

        // view property
        void viewProperty();
        void calculateTotal();
        void rebateAssumption();

        void addNew();
        void deleteSale();

        void filter(QComboBox *box, int row, QString columnName);

};

#endif // SALES_H
