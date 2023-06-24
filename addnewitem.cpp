#include "addnewitem.h"
#include "ui_addnewitem.h"

AddNewItem::AddNewItem(QSqlRelationalTableModel *model, QTableView *view, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewItem)
{


    ui->setupUi(this);

    database = QSqlDatabase::database("mydb");

    bool isOpen = database.open();

    if (!isOpen)
    {
        qDebug() << database.lastError().text();
    }

    tableModel = model;
    tableView = view;

    createView();
}

AddNewItem::~AddNewItem()
{
    delete ui;
}

QGroupBox *AddNewItem::buttonBox()
{
    QPushButton *submit = new QPushButton(tr("Submit"));
    QPushButton *cancel = new QPushButton(tr("Cancel"));

    connect(submit, &QPushButton::clicked, this, &AddNewItem::submitNewItem);
    connect(cancel, &QPushButton::clicked, this, &AddNewItem::close);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(submit);
    layout->addWidget(cancel);

    QGroupBox *box = new QGroupBox(tr("Add new item"));
    box->setLayout(layout);

    return box;
}

void AddNewItem::submitNewItem()
{
    if (!itemName->text().isEmpty() && !itemPrice->text().isEmpty())
    {
        QString name  = itemName->text();
        int     price = itemPrice->text().toInt();

        QString insert = QString("INSERT INTO mydb.inventory (Product, Price) "
                                 "VALUES ('%1', '%2')").arg(name).arg(price);

        database.exec(insert);
        tableModel->setTable("inventory");
        tableModel->select();
        tableView->setModel(tableModel);

        QMessageBox::StandardButton button;
        button = QMessageBox::information(this, "Item added", tr("%1 has been added, "
                                                                   "Click Ok to add more items, "
                                                                   "Click Close to close Window").arg(name),
                                          QMessageBox::Ok | QMessageBox::Close);

        if (button == QMessageBox::Close)
        {
            this->close();
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Empty Name Or Price");
    }
}

QGroupBox *AddNewItem::formBox()
{
    QLabel *item = new QLabel;
    item->setText("Item");
    QLabel *price = new QLabel;
    price->setText("Price");

    itemName = new QLineEdit;

    itemPrice = new QLineEdit;
    itemPrice->setPlaceholderText("Enter Price");
    itemPrice->setValidator(new QIntValidator(this));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(item, 0, 0);
    layout->addWidget(itemName, 0, 1);
    layout->addWidget(price, 1, 0);
    layout->addWidget(itemPrice, 1, 1);

    QGroupBox *box = new QGroupBox(tr("Enter item name and price"));
    box->setLayout(layout);

    return box;

}

void AddNewItem::createView()
{
    this->setWindowTitle("Add new item");

    QGroupBox *form = formBox();
    QGroupBox *button = buttonBox();

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(form, 0, 0);
    layout->addWidget(button, 1, 0);

}
