#include "membership.h"
#include "ui_membership.h"

Membership::Membership(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Membership)
{
    ui->setupUi(this);

    database = QSqlDatabase::database("mydb");

    if (!database.open())
    {
        qDebug() << database.lastError().text();
    }

    ui->setupUi(this);

    createView();
}

Membership::~Membership()
{
    delete ui;
}

QGroupBox *Membership::createfilterGroupBox()
{
    QLabel *expireDateLabel = new QLabel;
    expireDateLabel->setText("Membership Expire Date");
    expireDateLabel->setAlignment(Qt::AlignLeft);

    QLabel *membershipLabel = new QLabel;
    membershipLabel->setText("Membership");
    membershipLabel->setAlignment(Qt::AlignLeft);

    expireDateFilterBox = new QComboBox;
    membershipFilterBox = new QComboBox;

    for (int i = 0; i < model->rowCount(); i++)
    {
        QModelIndex expireDateIndex = model->index(i, 3);
        QModelIndex membershipIndex = model->index(i, 2);

        QString expireDateData = expireDateIndex.data().toString();
        QString memberData     = membershipIndex.data().toString();

        if (!expireDateData.isEmpty() && expireDateFilterBox->findText(expireDateData) == -1)
        {
            expireDateFilterBox->addItem(expireDateData);
        }

        if (!memberData.isEmpty() && membershipFilterBox->findText(memberData) == -1)
        {
            membershipFilterBox->addItem(memberData);
        }

    }

    expireDateFilterBox->insertItem(0, "<ALL>");
    expireDateFilterBox->setCurrentIndex(0);
    membershipFilterBox->insertItem(0, "<ALL>");
    membershipFilterBox->setCurrentIndex(0);

    connect(expireDateFilterBox, &QComboBox::currentIndexChanged, this, &Membership::changeExpireDateFilterBox);
    connect(membershipFilterBox, &QComboBox::currentIndexChanged, this, &Membership::changeMembershipFilterBox);


    QGridLayout *layout = new QGridLayout;
    layout->addWidget(expireDateLabel, 0, 0);
    layout->addWidget(expireDateFilterBox, 1, 0);
    layout->addWidget(membershipLabel, 0, 1);
    layout->addWidget(membershipFilterBox, 1, 1);

    QGroupBox *box = new QGroupBox(tr("Filter"));
    box->setLayout(layout);

    return box;

}

QGroupBox *Membership::createTableViewGroupBox()
{

    model = new QSqlTableModel(NULL, database);
    model->setTable("shoppersinfo");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Customer Name"));
    model->setHeaderData(1, Qt::Horizontal, tr("Customer Number"));
    model->setHeaderData(2, Qt::Horizontal, tr("Membership"));
    model->setHeaderData(3, Qt::Horizontal, tr("Expire date"));

    table = new QTableView;
    table->setModel(model);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setHidden(true);
    table->setSortingEnabled(true);
    table->setShowGrid(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(table, &QTableView::clicked, this, &Membership::operation);
    connect(table, &QTableView::activated, this, &Membership::operation);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(table);

    QGroupBox *box = new QGroupBox(tr("Membership Viewer"));
    box->setLayout(layout);

    return box;

}

QGroupBox *Membership::createRenewCostGroupBox()
{
    executiveMemberNumber     = new QLineEdit;
    regularMemberNumber      = new QLineEdit;
    excutiveRenewCostDisplay = new QLineEdit;
    regularRenewCostDisplay  = new QLineEdit;

    renewCost();

    executiveMemberNumber->setReadOnly(true);
    executiveMemberNumber->setAlignment(Qt::AlignRight);
    excutiveRenewCostDisplay->setReadOnly(true);
    excutiveRenewCostDisplay->setAlignment(Qt::AlignRight);
    regularMemberNumber->setReadOnly(true);
    regularMemberNumber->setAlignment(Qt::AlignRight);
    regularRenewCostDisplay->setReadOnly(true);
    regularRenewCostDisplay->setAlignment(Qt::AlignRight);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(executiveMemberNumber, 0, 1);
    layout->addWidget(excutiveRenewCostDisplay, 1, 1);
    layout->addWidget(regularMemberNumber, 0, 0);
    layout->addWidget(regularRenewCostDisplay, 1, 0);

    QGroupBox *box = new QGroupBox(tr("Membership renew"));
    box->setLayout(layout);

    return box;

}

QGroupBox *Membership::opertionGroupBox()
{
    addMember = new QPushButton(tr("&Add"));
    deleteMember = new QPushButton(tr("&Delete"));
    addMember->setEnabled(true);
    deleteMember->setEnabled(false);

    connect(addMember, &QPushButton::clicked, this, &Membership::addNewMember);
    connect(deleteMember, &QPushButton::clicked, this, &Membership::deleteSelectedMember);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(addMember, 0, 0);
    layout->addWidget(deleteMember, 1, 0);

    QGroupBox *box = new QGroupBox;
    box->setLayout(layout);

    return box;
}

void Membership::operation()
{
    deleteMember->setEnabled(true);
}

void Membership::renewCost()
{
    executiveMemberNumber->clear();
    excutiveRenewCostDisplay->clear();
    regularMemberNumber->clear();
    regularRenewCostDisplay->clear();

    int executiveCount{0};
    int regularCount{0};

    for (int i = 0; i < model->rowCount(); i++)
    {
        QModelIndex index = model->index(i, 2);

        QString data = index.data().toString();

        if (data == "Executive")
        {
            executiveCount += 1;
        }

        if (data == "Regular")
        {
            regularCount += 1;
        }
    }

    executiveMember    = executiveCount;
    regularMember      = regularCount;
    executiveRenewCost = executiveCount * 120.00;
    regularRenewCost   = regularCount * 65.00;

    executiveMemberNumber->setText("Total executive member number: " + QString::number(executiveMember));
    excutiveRenewCostDisplay->setText("Executive renew cost: " + QString::number(executiveRenewCost));
    regularMemberNumber->setText("Total regular member number: " + QString::number(regularMember));
    regularRenewCostDisplay->setText("Regular renew cost: " + QString::number(regularRenewCost));
}

void Membership::changeMembershipFilterBox(int row)
{
    runFilter(membershipFilterBox, row, "Membership");
}

void Membership::changeExpireDateFilterBox(int row)
{
    runFilter(expireDateFilterBox, row, "MembershipExpireDate");
}

void Membership::runFilter(QComboBox *box, int row, QString columnName)
{
    QString selected = box->itemText(row);
    QString sqlFilter;

    if (row > 0)
    {
        if (columnName.contains("Date"))
        {
            sqlFilter = QString("%1 = STR_TO_DATE('%2', '%Y-%m-%d')").arg(columnName).arg(selected);
        }
        else
        {
            sqlFilter = QString("%1 = '%2'").arg(columnName).arg(selected);
        }

        model->setFilter(sqlFilter);

    }
    else
    {
        model->setFilter(QString());
    }

    renewCost();

}

void Membership::addNewMember()
{
    AddNew addNew(model, table);

    int accepted = addNew.exec();

    if (accepted == 1)
    {
        model->setTable("shoppersinfo");
        model->select();
        table->setModel(model);
    }
}

void Membership::deleteSelectedMember()
{
    QModelIndexList selection = table->selectionModel()->selectedRows(0);

    if (!selection.isEmpty())
    {
        QString selectedCustomerName = selection.at(0).data().toString();
        qDebug() << "Delete: " + selectedCustomerName;

        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("Delete Member"),
                                       tr("Are you sure you want to "
                                          "delete '%1'").arg(selectedCustomerName),
                                       QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes)
        {
            removeMemberFromDatabase(selection.at(0));
            model->setTable("shoppersinfo");
            model->select();
            table->setModel(model);
        }
    }

}

void Membership::removeMemberFromDatabase(QModelIndex index)
{
    model->removeRow(index.row());
}


void Membership::createView()
{
    this->resize(QSize(1000, 800));
    this->setWindowTitle("Membership Viewer");

    QGroupBox *table  = createTableViewGroupBox();
    QGroupBox *filter = createfilterGroupBox();
    QGroupBox *display = createRenewCostGroupBox();
    QGroupBox *operation = opertionGroupBox();

    QGridLayout *buttonLayout = new QGridLayout;
    buttonLayout->addWidget(operation, 0, 0);
    buttonLayout->addWidget(display, 0, 1);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(filter, 0, 0);
    layout->addWidget(table, 1, 0);
    layout->addLayout(buttonLayout, 2, 0);
}
