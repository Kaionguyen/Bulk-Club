#ifndef MEMBERSHIP_H
#define MEMBERSHIP_H

#include <QDialog>
#include <QDebug>
#include <QHeaderView>
#include <QTableView>
#include <QGroupBox>
#include <QBoxLayout>
#include <QComboBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QStandardItem>

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlTableModel>

#include "databasehandler.h"
#include "addnew.h"

namespace Ui {
    class Membership;
}

class Membership : public QDialog
{
        Q_OBJECT

    public:
        explicit Membership(QWidget *parent = nullptr);
        ~Membership();

    private slots:
        void changeMembershipFilterBox(int row);
        void changeExpireDateFilterBox(int row);

    private:
        Ui::Membership *ui;

        int    executiveMember;
        int    regularMember;
        double executiveRenewCost;
        double regularRenewCost;

        QLineEdit *executiveMemberNumber;
        QLineEdit *regularMemberNumber;
        QLineEdit *excutiveRenewCostDisplay;
        QLineEdit *regularRenewCostDisplay;

        QPushButton *addMember;
        QPushButton *deleteMember;

        QSqlTableModel *model;
        QSqlDatabase database;

        QTableView *table;

        QComboBox *expireDateFilterBox;
        QComboBox *membershipFilterBox;

        QGroupBox *createfilterGroupBox();
        QGroupBox *createTableViewGroupBox();
        QGroupBox *createRenewCostGroupBox();
        QGroupBox *opertionGroupBox();

        void renewCost();
        void createView();
        void runFilter(QComboBox *box, int row, QString columnName);
        void operation();
        void addNewMember();
        void deleteSelectedMember();
        void removeMemberFromDatabase(QModelIndex index);


};

#endif // MEMBERSHIP_H
