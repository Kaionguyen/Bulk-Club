#ifndef ADDNEW_H
#define ADDNEW_H

#include <QtWidgets>
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

#include <QSql>
#include <QSqlError>
#include <QModelIndex>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>


namespace Ui {
    class AddNew;
}

class AddNew : public QDialog
{
        Q_OBJECT

    public:
        explicit AddNew(QSqlTableModel *model, QTableView *view, QWidget *parent = nullptr);
        ~AddNew();

    private:
        Ui::AddNew *ui;

        QLineEdit *memberName;
        QLineEdit *memberNumber;
        QComboBox *membership;
        QDateEdit *expireDate;

        QSqlDatabase   database;
        QSqlTableModel *tableModel;
        QTableView *tableView;

        QGroupBox *createInputFormBox();
        QGroupBox *createButtonBox();

        QDateEdit *createDateSpinBox();

        QComboBox *createMemberTypeBox();

        void createView();
        void submitNewMember();




};

#endif // ADDNEW_H
