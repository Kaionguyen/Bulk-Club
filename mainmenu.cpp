#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    // Create connection to database
    shared_handlers = std::make_shared<DatabaseHandler>();
    shared_fileProcess = std::make_shared<FileProcess>();

    shared_handlers->connectionToDatabase();
    shared_handlers->createDatabase();
    shared_handlers->createTables();
    shared_handlers->createSaleReportTable();
    shared_handlers->selectDatabase();

    shared_fileProcess->readFiles();

    shared_handlers->loadDataIntoCombinedTable();

    menuView();


}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::menuView()
{
    QSize size(200, 200);
    this->setWindowTitle("Bulk Club Management System");

    // Create sales report button
    sales = new QPushButton;
    sales->setText("Generate Sales Report");
    sales->setMinimumSize(size);

    // Create membership report button
    member = new QPushButton;
    member->setText("View Membership");
    member->setMinimumSize(size);

    // Create Inventory button
    inventry = new QPushButton;
    inventry->setText("View Inventory List");
    inventry->setMinimumSize(size);

    connect(sales, &QPushButton::clicked, this, &MainMenu::openSalesReport);
    connect(member, &QPushButton::clicked, this, &MainMenu::openMembership);
    connect(inventry, &QPushButton::clicked, this, &MainMenu::openInventory);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(sales, 0, 0);
    layout->addWidget(member, 0, 1);
    layout->addWidget(inventry, 0 , 2);

}

void MainMenu::openSalesReport()
{
    Sales sales;
    sales.exec();
}

void MainMenu::openMembership()
{

    Membership membership;
    membership.exec();
}

void MainMenu::openInventory()
{
    Inventory inventory;
    inventory.exec();
}
