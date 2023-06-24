#include "addnew.h"
#include "ui_addnew.h"

AddNew::AddNew(QSqlTableModel *model, QTableView *view, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNew)
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

AddNew::~AddNew()
{
    delete ui;
}

QGroupBox *AddNew::createInputFormBox()
{
    QLabel *customerName = new QLabel(tr("Name:"));
    QLabel *customerNumber = new QLabel(tr("Number:"));
    QLabel *memberType     = new QLabel(tr("Type:"));
    QLabel *memberExpireDate = new QLabel(tr("Expire date:"));

    memberName   = new QLineEdit;
    membership   = createMemberTypeBox();
    memberNumber = new QLineEdit;
    memberNumber->setValidator(new QIntValidator(0, 999999999, this));
    memberNumber->setPlaceholderText("Please enter a postive customer number");
    expireDate   = createDateSpinBox();

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(customerName, 0, 0);
    layout->addWidget(memberName, 0, 1);
    layout->addWidget(customerNumber, 1, 0);
    layout->addWidget(memberNumber, 1, 1);
    layout->addWidget(memberType, 2, 0);
    layout->addWidget(membership, 2, 1);
    layout->addWidget(memberExpireDate, 3, 0);
    layout->addWidget(expireDate, 3, 1);

    QGroupBox *box = new QGroupBox(tr("Add Member"));
    box->setLayout(layout);

    return box;

}

QGroupBox *AddNew::createButtonBox()
{
    QPushButton *submit = new QPushButton(tr("Submit"));
    QPushButton *close  = new QPushButton(tr("Close"));

    connect(submit, &QPushButton::clicked, this, &AddNew::submitNewMember);
    connect(close, &QPushButton::clicked, this, &AddNew::close);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(submit, 0, 0);
    layout->addWidget(close, 0 ,1);

    QGroupBox *box = new QGroupBox;
    box->setLayout(layout);

    return box;
}

QDateEdit *AddNew::createDateSpinBox()
{
    expireDate = new QDateEdit(QDate::currentDate());
    expireDate->setDateRange(QDate(1900, 1, 1), QDate(2050, 1, 1));

    return expireDate;

}

QComboBox *AddNew::createMemberTypeBox()
{
    membership = new QComboBox;
    membership->addItem("Executive");
    membership->addItem("Regular");

    return membership;
}

void AddNew::submitNewMember()
{
    if (memberName->text().isEmpty() || memberNumber->text().isEmpty())
    {
        QMessageBox::warning(this, "Error", "customer name or Number is empty");

        return;
    }

    QString name     = memberName->text();
    int customerNo   = memberNumber->text().toInt();
    QString memberTy = membership->currentText();
    QString date     = expireDate->text();

    QString insertNewRecord = QString("INSERT INTO mydb.shoppersinfo (CustomerName, CustomerNumber, Membership, MembershipExpireDate) "
                              "VALUES ('%1', '%2', '%3', STR_TO_DATE('%4', '%m/%d/%Y'))").arg(name).arg(customerNo).arg(memberTy).arg(date);

    database.exec(insertNewRecord);
    tableModel->setTable("shoppersinfo");
    tableModel->select();
    tableView->setModel(tableModel);

    QMessageBox::StandardButton button;
    button = QMessageBox::information(this, "Member added", tr("%1 has been added, "
                                                               "Click Ok to add more members, "
                                                               "Click Close to close Window").arg(name),
                                      QMessageBox::Ok | QMessageBox::Close);

    if (button == QMessageBox::Close)
    {
        this->close();
    }

}

void AddNew::createView()
{
    this->setFixedHeight(380);
    this->setWindowTitle("Add Bulk club member");

    QGroupBox *inputform = createInputFormBox();
    inputform->setMinimumHeight(300);
    QGroupBox *buttons   = createButtonBox();

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(inputform, 0, 0);
    layout->addWidget(buttons, 1, 0);
}
