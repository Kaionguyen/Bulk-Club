#include "sales.h"
#include "ui_sales.h"

Sales::Sales(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sales)
{
    database = QSqlDatabase::database("mydb");

    if (!database.open())
    {
        qDebug() << database.lastError().text();
    }

    ui->setupUi(this);

    viewProperty();

}

Sales::~Sales()
{
    delete ui;
}

QGroupBox *Sales::createTableViewGroupBox()
{

    tableModel = new QSqlTableModel(NULL, database);
    tableModel->setTable("salesreport");
    tableModel->select();
    tableModel->setHeaderData(1, Qt::Horizontal, tr("Purchase Date"));
    tableModel->setHeaderData(2, Qt::Horizontal, tr("Customer Name"));
    tableModel->setHeaderData(3, Qt::Horizontal, tr("Customer Number"));
    tableModel->setHeaderData(4, Qt::Horizontal, tr("Product"));
    tableModel->setHeaderData(5, Qt::Horizontal, tr("Price"));
    tableModel->setHeaderData(6, Qt::Horizontal, tr("Quantity"));
    tableModel->setHeaderData(7, Qt::Horizontal, tr("Membership"));
    tableModel->setHeaderData(8, Qt::Horizontal, tr("Membership Expire Date"));


    tableView = new QTableView;
    tableView->setModel(tableModel);
    tableView->setShowGrid(true);
    tableView->setSortingEnabled(true);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setHidden(true);
    tableView->hideColumn(0); // hide ID column

    QGroupBox *tableViewBox = new QGroupBox(tr("Sales Report"));

    QBoxLayout *layout      = new QBoxLayout(QBoxLayout::TopToBottom);
    layout->addWidget(tableView);

    tableViewBox->setLayout(layout);

    return tableViewBox;

}

QGroupBox *Sales::createFilterGroupBox()
{

    tableModel->sort(0, Qt::AscendingOrder);
    // Set filter options for first three options
    auto filterBoxOne = new QBoxLayout(QBoxLayout::LeftToRight);
    purchaseDate = setUpComboBox(purchaseDate, 1, "Purchase Date");
    customerName = setUpComboBox(customerName, 2, "Customer Name");
    customerNumber = setUpComboBox(customerNumber, 3, "Customer Number");

    filterBoxOne->addWidget(purchaseDate);
    filterBoxOne->addWidget(customerName);
    filterBoxOne->addWidget(customerNumber);

    // Set filter options for second three options
    auto filterBoxTwo = new QBoxLayout(QBoxLayout::LeftToRight);
    product = setUpComboBox(product, 4, "Product");
    membership = setUpComboBox(membership, 7, "Membership");
    membershipExpireDate = setUpComboBox(membershipExpireDate, 8, "Expire Date");

    filterBoxTwo->addWidget(product);
    filterBoxTwo->addWidget(membership);
    filterBoxTwo->addWidget(membershipExpireDate);

    QGroupBox *filterBox = new QGroupBox(tr("Filters"));

    QGridLayout *layout  = new QGridLayout;
    layout->addLayout(filterBoxOne, 0, 0);
    layout->addLayout(filterBoxTwo, 1, 0);

    filterBox->setLayout(layout);

    // Link signals and slots
    connect(purchaseDate, &QComboBox::currentIndexChanged, this, &Sales::purchaseDateFilter);
    connect(customerName, &QComboBox::currentIndexChanged, this, &Sales::customerNameFilter);
    connect(customerNumber, &QComboBox::currentIndexChanged, this, &Sales::customerNumberFilter);
    connect(product, &QComboBox::currentIndexChanged, this, &Sales::productFilter);
    connect(membership, &QComboBox::currentIndexChanged, this, &Sales::membershipFilter);
    connect(membershipExpireDate, &QComboBox::currentIndexChanged, this, &Sales::expireDateFilter);

    return filterBox;
}

QGroupBox *Sales::buttonBox()
{
    QPushButton *addNewSale = new QPushButton(tr("Add"));
    QPushButton *deleteSale = new QPushButton(tr("Delete"));

    connect(addNewSale, &QPushButton::clicked, this, &Sales::addNew);
    connect(deleteSale, &QPushButton::clicked, this, &Sales::deleteSale);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(addNewSale, 0 , 0);
    layout->addWidget(deleteSale, 1, 0);

    QGroupBox *box = new QGroupBox(tr("Add/Delete Sale"));
    box->setLayout(layout);

    return box;
}

void Sales::purchaseDateFilter(int row)
{
    filter(purchaseDate, row, "PurchaseDate");
    renewCostEdit->hide();
    rebateLabel->hide();
    recommendation->hide();
}

void Sales::customerNameFilter(int row)
{
    filter(customerName, row, "CustomerName");
    if (customerName->currentIndex() != 0)
    {
        rebateAssumption();
    }
    else
    {
        renewCostEdit->hide();
        rebateLabel->hide();
        recommendation->hide();
    }

}

void Sales::customerNumberFilter(int row)
{
    filter(customerNumber, row, "CustomerNumber");
    renewCostEdit->hide();
    rebateLabel->hide();
    recommendation->hide();
}

void Sales::productFilter(int row)
{
    filter(product, row, "Product");
    renewCostEdit->hide();
    rebateLabel->hide();
    recommendation->hide();
}

void Sales::membershipFilter(int row)
{
    filter(membership, row, "Membership");
    renewCostEdit->hide();
    recommendation->hide();
    if (membership->currentText() == "Executive")
    {
        rebateLabel->show();
    }
    else
    {
        rebateLabel->hide();
    }

}

void Sales::expireDateFilter(int row)
{
    filter(membershipExpireDate, row, "MembershipExpireDate");
    renewCostEdit->hide();
    rebateLabel->hide();
    recommendation->hide();
}

void Sales::addNew()
{
    AddNewSale newSale(tableModel, tableView);
    newSale.exec();
}

void Sales::deleteSale()
{
    QModelIndexList selection = tableView->selectionModel()->selectedRows(0);

    if (!selection.isEmpty())
    {
        QString selectedCustomerName = selection.at(0).siblingAtColumn(2).data().toString();
        QString selectedPurchaseDate = selection.at(0).siblingAtColumn(1).data().toString();
        qDebug() << "Delete: " + selectedCustomerName;

        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("Delete Member"),
                                       tr("Are you sure you want to "
                                          "delete '%1''s purchase at '%2'").arg(selectedCustomerName).arg(selectedPurchaseDate),
                                       QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes)
        {
            tableModel->removeRow(selection.at(0).row());
            tableModel->setTable("salesreport");
            tableModel->select();
            tableView->setModel(tableModel);
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Please select a row to delete");
    }
}

void Sales::filter(QComboBox *box, int row, QString columnName)
{
    QString filterData;

    if (row > 0)
    {
        QString data = box->itemText(row);
        if (columnName.contains("Date"))
        {
            filterData = QString("DATE(%1) = STR_TO_DATE('%2', '%Y-%m-%d')").arg(columnName).arg(data);
        }
        else
        {
           filterData = QString("%1 = '%2'").arg(columnName).arg(data);
        }
        tableModel->setFilter(filterData);
        calculateTotal();
    }
    else if (row == 0)
    {
        tableModel->setFilter(QString());
        calculateTotal();
    }
    else
    {
        return;
    }
}

QComboBox *Sales::setUpComboBox(QComboBox *box, int col, QString emptySelection)
{
    box = new QComboBox;

    for (int i = 0; i < tableModel->rowCount(); i++)
    {
        QString data= tableModel->data(tableModel->index(i, col)).toString();

        if (!data.isEmpty() && box->findText(data) == -1)
        {
            box->addItem(data);
        }
    }

    box->insertItem(0, emptySelection);
    box->setCurrentIndex(0);

    return box;
}

QGroupBox *Sales::revenueInfoGroupBox()
{

    QFont font("segoe UI", 14, QFont::Light);

    subTotalLabel  = new QLineEdit;
    subTotalLabel->setReadOnly(true);
    subTotalLabel->setAlignment(Qt::AlignRight);
    subTotalLabel->setFont(font);

    taxLabel       = new QLineEdit;
    taxLabel->setReadOnly(true);
    taxLabel->setAlignment(Qt::AlignRight);
    taxLabel->setFont(font);

    totalRevLabel  = new QLineEdit;
    totalRevLabel->setReadOnly(true);
    totalRevLabel->setAlignment(Qt::AlignRight);
    totalRevLabel->setFont(font);

    renewCostEdit = new QLineEdit;
    renewCostEdit->setReadOnly(true);
    renewCostEdit->setAlignment(Qt::AlignRight);
    renewCostEdit->setFont(font);
    renewCostEdit->hide();

    rebateLabel  = new QLineEdit;
    rebateLabel->setReadOnly(true);
    rebateLabel->setAlignment(Qt::AlignRight);
    rebateLabel->setFont(font);
    rebateLabel->hide();

    recommendation = new QLineEdit;
    recommendation->setReadOnly(true);
    recommendation->setAlignment(Qt::AlignRight);
    recommendation->setFont(font);
    recommendation->hide();

    calculateTotal();

    auto revInfoLayout = new QGridLayout;

    revInfoLayout->addWidget(subTotalLabel, 0, 0);
    revInfoLayout->addWidget(taxLabel, 1, 0);
    revInfoLayout->addWidget(totalRevLabel, 2, 0);
    revInfoLayout->addWidget(renewCostEdit, 3, 0);
    revInfoLayout->addWidget(rebateLabel, 4, 0);
    revInfoLayout->addWidget(recommendation, 5, 0);
    revInfoLayout->setAlignment(Qt::AlignRight);

    QGroupBox *box = new QGroupBox(tr("Total Sale/Rebate"));

    box->setLayout(revInfoLayout);

    return box;
}

void Sales::calculateTotal()
{

    double price{0};
    double quantity{0};

    subTotal = 0;
    executiveTotal = 0;
    totalRevenue = 0;
    renewCost = 0;

    subTotalLabel->clear();
    taxLabel->clear();
    totalRevLabel->clear();
    rebateLabel->clear();

    for (int i = 0; i < tableModel->rowCount(); i++)
    {
        double result;

        QModelIndex priceIndex = tableModel->index(i, 5);
        QModelIndex quantityIndex = tableModel->index(i, 6);

        price = priceIndex.data().toDouble();
        quantity = quantityIndex.data().toDouble();

        result = (price * quantity);

        subTotal += result;

    }

    taxTotal     = subTotal * 0.0775;
    totalRevenue = subTotal * 1.0775;
    rebate       = subTotal * 0.02;

    subTotalLabel->setText("Grand Total: $" + QString::number(subTotal));
    taxLabel->setText("Tax 7.75% : $" + QString::number(taxTotal));
    totalRevLabel->setText("Total Revenue: $" + QString::number(totalRevenue));
    rebateLabel->setText("(Rebate with an excutive member 2%: $" + QString::number(rebate) + ")");

}

void Sales::rebateAssumption()
{

    if (tableModel->data(tableModel->index(0, 7)).toString() == "Regular")
    {
        renewCost = 65.00;
    }
    else
    {
        renewCost = 120.00;
    }
    renewCostEdit->setText("(Renew Cost: $" + QString::number(renewCost) + ")");
    renewCostEdit->show();
    rebateLabel->show();

    QString recommend;
    if (renewCost > rebate)
    {
        recommend = "Renew cost is higher than rebate received, recommend regular membership!";
    }
    else
    {
        recommend = "Renew cost is lower than rebate received, recommend executive membership!";
    }

    recommendation->setText(recommend);
    recommendation->show();
}

void Sales::viewProperty()
{
    this->resize(QSize(1500, 1200));
    setWindowTitle("Sales Report");

    QGroupBox *dataTable = createTableViewGroupBox();
    QGroupBox *filter = createFilterGroupBox();
    QGroupBox *revenue = revenueInfoGroupBox();
    QGroupBox *addAndDel = buttonBox();

    QGridLayout *filterAndTableLayout = new QGridLayout;
    filterAndTableLayout->addWidget(filter, 0, 0);
    filterAndTableLayout->addWidget(dataTable,1 , 0);

    QGridLayout *buttonAndRevenueLayout = new QGridLayout;
    buttonAndRevenueLayout->addWidget(addAndDel, 0, 0);
    buttonAndRevenueLayout->addWidget(revenue, 0, 1);

    QGridLayout *layout = new QGridLayout(this);
    layout->addLayout(filterAndTableLayout, 0 , 0);
    layout->addLayout(buttonAndRevenueLayout, 1, 0);
}


