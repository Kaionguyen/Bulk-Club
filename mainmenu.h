#ifndef MAINMENU_H
#define MAINMENU_H

#include <memory>

#include <QDialog>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>

#include "databasehandler.h"
#include "fileprocess.h"
#include "sales.h"
#include "membership.h"
#include "inventory.h"

namespace Ui {
    class MainMenu;
}

class MainMenu : public QDialog
{
        Q_OBJECT

    public:
        explicit MainMenu(QWidget *parent = nullptr);
        ~MainMenu();

    private:
        Ui::MainMenu *ui;
//        QDialog *salesinfo;
//        QDialog *memberinfo;

        void menuView();
        void openSalesReport();
        void openMembership();
        void openInventory();

        std::shared_ptr<DatabaseHandler> shared_handlers;
        std::shared_ptr<FileProcess> shared_fileProcess;

        QSqlQueryModel *model;

        QPushButton *sales;
        QPushButton *member;
        QPushButton *inventry;
};

#endif // MAINMENU_H
