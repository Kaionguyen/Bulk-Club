#ifndef ADDNEWSALE_H
#define ADDNEWSALE_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDate>
#include <QComboBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QDebug>
#include <QTableView>
#include <QListWidget>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>

namespace Ui {
    class AddNewSale;
}

class AddNewSale : public QDialog
{
        Q_OBJECT

    public:
        explicit AddNewSale(QSqlTableModel *model, QTableView *view, QWidget *parent = nullptr);
        ~AddNewSale();

    private:
        Ui::AddNewSale *ui;

        QSqlDatabase database;

        QSqlTableModel *salesModel;
        QSqlTableModel *memberModel;
        QSqlTableModel *inventoryModel;

        QDateEdit  *purchaseDateBox;
        QComboBox  *memberNameBox;
        QLineEdit  *memberNumberLine;
        QLineEdit  *memberTypeEdit;
        QLineEdit  *memberExpireLine;
        QComboBox  *productBox;
        QLineEdit  *priceLine;
        QLineEdit  *quantityEdit;



        QGroupBox *formBox();
        QGroupBox *buttonBox();

        QTableView *tableView;

        void createView();
        void createNewSale();
        void findMemberInfo(int row);
        void findProductInfo(int row);
        void runFind(int row, QString boxChanged);
};

#endif // ADDNEWSALE_H
