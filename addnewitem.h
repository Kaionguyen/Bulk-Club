#ifndef ADDNEWITEM_H
#define ADDNEWITEM_H

#include <QWidget>
#include <QDialog>
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

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>

namespace Ui {
    class AddNewItem;
}

class AddNewItem : public QDialog
{
        Q_OBJECT

    public:
        explicit AddNewItem(QSqlRelationalTableModel *model, QTableView *view, QWidget *parent = nullptr);
        ~AddNewItem();

    private:
        Ui::AddNewItem *ui;

        QSqlDatabase database;
        QSqlRelationalTableModel *tableModel;
        QTableView *tableView;

        QLineEdit *itemName;
        QLineEdit *itemPrice;


        void createView();
        void submitNewItem();

        QGroupBox *buttonBox();
        QGroupBox *formBox();
};

#endif // ADDNEWITEM_H
