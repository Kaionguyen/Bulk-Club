#include "inventory.h"
#include "ui_inventory.h"

Inventory::Inventory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Inventory)
{
    ui->setupUi(this);

    database = QSqlDatabase::database("mydb");

    bool isOpen = database.open();

    if (!isOpen)
    {
        qDebug() << database.lastError().text();
    }

    createView();
}

Inventory::~Inventory()
{
    delete ui;
}

QGroupBox *Inventory::inventoryTable()
{
    model = new QSqlRelationalTableModel(NULL, database);
    model->setTable("inventory");
    model->select();

    inventoryTableView = new QTableView;
    inventoryTableView->setModel(model);
    inventoryTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    inventoryTableView->setSortingEnabled(true);
    inventoryTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    inventoryTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(inventoryTableView);

    QGroupBox *box = new QGroupBox(tr("Inventory"));
    box->setLayout(layout);

    return box;
}


QGroupBox *Inventory::buttonBox()
{
    QPushButton *addItem = new QPushButton(tr("Add"));
    QPushButton *deleteItem = new QPushButton(tr("Delete"));

    connect(addItem, &QPushButton::clicked, this, &Inventory::addNewItem);
    connect(deleteItem, &QPushButton::clicked, this, &Inventory::deleteItem);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(addItem, 0 , 0);
    layout->addWidget(deleteItem, 0 ,1);

    QGroupBox *box = new QGroupBox(tr("Add/Delete Item"));
    box->setLayout(layout);

    return box;

}

void Inventory::createView()
{
    this->setWindowTitle("Bulk Club Inventory");

    QGroupBox *table = inventoryTable();
    QGroupBox *button = buttonBox();

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(table);
    layout->addWidget(button);

}

void Inventory::addNewItem()
{
    AddNewItem addNewItem(model, inventoryTableView);
    int accepted = addNewItem.exec();

    if (accepted == 1)
    {
        int lastRow = model->rowCount() - 1;
        inventoryTableView->selectRow(lastRow);
        inventoryTableView->scrollToBottom();
    }
}

void Inventory::deleteItem()
{
    QModelIndexList selection = inventoryTableView->selectionModel()->selectedRows(0);

    if (!selection.isEmpty())
    {
        QString selectedProduct = selection.at(0).data().toString();

        QMessageBox::Button button;

        button = QMessageBox::question(this, "Delete Item",
                                       tr("Confirm to delete the item %1").arg(selectedProduct),
                                       QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::Yes)
        {
            model->removeRow(selection.at(0).row());
            model->setTable("inventory");
            model->select();
            inventoryTableView->setModel(model);
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Select an item to delete");
    }

}
