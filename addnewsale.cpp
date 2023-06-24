#include "addnewsale.h"
#include "ui_addnewsale.h"

AddNewSale::AddNewSale(QSqlTableModel *model, QTableView *view, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewSale)
{
    ui->setupUi(this);

    database = QSqlDatabase::database("mydb");

    bool isOpen = database.open();

    if (!isOpen)
    {
        qDebug() << database.lastError().text();
    }

    salesModel = model;
    tableView = view;

    createView();
}

AddNewSale::~AddNewSale()
{
    delete ui;
}

QGroupBox *AddNewSale::formBox()
{
    memberModel = new QSqlTableModel(NULL, database);
    memberModel->setTable("shoppersinfo");
    memberModel->select();
    inventoryModel = new QSqlTableModel(NULL, database);
    inventoryModel->setTable("inventory");
    inventoryModel->select();

    QLabel *purchaseDateLabel = new QLabel(tr("Purchase Date"));
    purchaseDateBox = new QDateEdit(QDate::currentDate());
    purchaseDateBox->setDateRange(QDate(2019, 1, 1), QDate::currentDate());


    QLabel *memberNameLabel = new QLabel(tr("Member Name:"));
    memberNameBox = new QComboBox;
    for (int i = 0; i < memberModel->rowCount(); i++)
    {
        QString data = memberModel->data(memberModel->index(i, 0)).toString();

        if (memberNameBox->findData(data) == -1)
        {
            memberNameBox->addItem(data);
        }


    }

    QLabel *memberNumberLabel = new QLabel(tr("Member Number:"));
    memberNumberLine = new QLineEdit;
    memberNumberLine->setReadOnly(true);

    QLabel *memberTypeLabel = new QLabel(tr("Member"));
    memberTypeEdit = new QLineEdit;
    memberTypeEdit->setReadOnly(true);

    QLabel *expireLineLabel = new QLabel(tr("Expire Date:"));
    memberExpireLine = new QLineEdit;
    memberExpireLine->setReadOnly(true);

    QLabel *productBoxLabel = new QLabel(tr("Product:"));
    productBox = new QComboBox;
    for (int i = 0; i < inventoryModel->rowCount(); i++)
    {
        QString data = inventoryModel->data(inventoryModel->index(i, 0)).toString();

        if (productBox->findData(data) == -1)
        {
            productBox->addItem(data);
        }
    }

    QLabel *priceLineLabel = new QLabel(tr("Price"));
    priceLine = new QLineEdit;
    priceLine->setReadOnly(true);

    QLabel *quantityPurchased = new QLabel(tr("Quantity"));
    quantityEdit = new QLineEdit;
    quantityEdit->setValidator(new QIntValidator(0, 9999999, this));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(purchaseDateLabel, 0, 0);
    layout->addWidget(purchaseDateBox, 0 , 1);
    layout->addWidget(memberNameLabel, 1 , 0);
    layout->addWidget(memberNameBox, 1, 1);
    layout->addWidget(productBoxLabel, 2, 0);
    layout->addWidget(productBox, 2, 1);
    layout->addWidget(priceLineLabel, 3, 0);
    layout->addWidget(priceLine, 3, 1);
    layout->addWidget(quantityPurchased, 4, 0);
    layout->addWidget(quantityEdit, 4, 1);
    layout->addWidget(memberNumberLabel, 5, 0);
    layout->addWidget(memberNumberLine, 5, 1);
    layout->addWidget(memberTypeLabel, 6, 0);
    layout->addWidget(memberTypeEdit, 6, 1);
    layout->addWidget(expireLineLabel, 7, 0);
    layout->addWidget(memberExpireLine, 7, 1);

    QGroupBox *box = new QGroupBox(tr("Select Member and Product"));
    box->setLayout(layout);

    connect(memberNameBox, &QComboBox::currentIndexChanged, this, &AddNewSale::findMemberInfo);
    connect(productBox, &QComboBox::currentIndexChanged, this, &AddNewSale::findProductInfo);

    return box;
}

QGroupBox *AddNewSale::buttonBox()
{
    QPushButton *submit = new QPushButton(tr("Submit"));
    QPushButton *close = new QPushButton(tr("Close"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(submit, 0, 0);
    layout->addWidget(close, 0, 1);

    connect(submit, &QPushButton::clicked, this, &AddNewSale::createNewSale);
    connect(close, &QPushButton::clicked, this, &AddNewSale::close);

    QGroupBox *box = new QGroupBox;
    box->setLayout(layout);

    return box;

}

void AddNewSale::createView()
{
    QGroupBox *form = formBox();
    QGroupBox *button = buttonBox();

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(form, 0, 0);
    layout->addWidget(button, 1, 0);
}

void AddNewSale::findMemberInfo(int row)
{
    runFind(row, "CustomerName");
}

void AddNewSale::findProductInfo(int row)
{
    runFind(row, "Product");

}

void AddNewSale::runFind(int row, QString sqlColumnName)
{
    QString data;
    if (sqlColumnName == "CustomerName")
    {
        data = memberNameBox->itemText(row);
    }
    else if (sqlColumnName == "Product")
    {
        data = productBox->itemText(row);
    }

    QString filter = QString("%1 = '%2'").arg(sqlColumnName).arg(data);

    if (sqlColumnName == "CustomerName")
    {
        memberModel->setFilter(filter);
        memberNumberLine->setText(memberModel->data(memberModel->index(0, 1)).toString());
        memberTypeEdit->setText(memberModel->data(memberModel->index(0, 2)).toString());
        memberExpireLine->setText(memberModel->data(memberModel->index(0, 3)).toString());
    }
    else if (sqlColumnName == "Product")
    {
        inventoryModel->setFilter(filter);
        priceLine->setText(inventoryModel->data(inventoryModel->index(0, 1)).toString());
    }


}

void AddNewSale::createNewSale()
{

    int id = salesModel->rowCount() + 1;

    QString purchaseDate = purchaseDateBox->text();
    QSqlQuery query(database);
    QString salesReportQuery = QString("INSERT INTO salesreport (ID, PurchaseDate,CustomerName, CustomerNumber, Membership, MembershipExpireDate, Product, Price, Quantity) "
                                       "VALUES ('%1', STR_TO_DATE('%2', '%m/%d/%Y'), '%3', '%4', '%5', STR_TO_DATE('%6', '%Y-%m-%d'), '%7', '%8', '%9') "
                                       "ON DUPLICATE KEY UPDATE "
                                       "PurchaseDate = STR_TO_DATE('%2', '%m/%d/%Y'), "
                                       "CustomerNumber = '%3', "
                                       "Product = '%4', "
                                       "Quantity = '%6';"
                                       ).
                                        arg(id).arg(purchaseDate).arg(memberNameBox->currentText()).
                                        arg(memberNumberLine->text()).arg(memberTypeEdit->text()).arg(memberExpireLine->text()).arg(productBox->currentText()).
                                        arg(priceLine->text()).arg(quantityEdit->text());
    query.prepare(salesReportQuery);
    bool isExec = query.exec();

    if (!isExec)
    {
        qDebug() << query.lastError().text();
    }

    salesModel->setTable("salesreport");
    salesModel->select();
    tableView->setModel(salesModel);

    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, "Sale Added", tr("Sale has been added, click continue to add more or close window"),
                                   QMessageBox::Yes | QMessageBox::Close);

    if (button == QMessageBox::Close)
    {
        this->close();
    }
}
