#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    createViews();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createViews()
{
    this->setWindowTitle("Login");
    this->setFixedSize(800, 600);

    // Set backgroup in login window
    auto backgroundLabel = new QLabel(this);
    backgroundLabel->resize(this->size());
    backgroundLabel->setStyleSheet("background-color: rgb(255, 255, 255);");

    // Set System name
    auto titleLabel = new QLabel;
    titleLabel->setText("Bulk Club Sales System");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font("segoe UI", 18, QFont::Light);
    titleLabel->setFont(font);

    // Set username label and enter box
    auto usernameLabel = new QLabel;
    usernameLabel->setMaximumWidth(100);
    usernameLabel->setText("Username:");
    usernameEditLine = new QLineEdit;
    usernameEditLine->setMaximumWidth(300);

    // Set password label and enter box
    auto passwordLaebl = new QLabel;
    passwordLaebl->setMaximumWidth(100);
    passwordLaebl->setText("Password:");
    passwordEditLine = new QLineEdit;
    passwordEditLine->setEchoMode(QLineEdit::Password);
    passwordEditLine->setMaximumWidth(300);

    // Layout for username input
    auto usernameLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameEditLine);
    usernameLayout->setSpacing(8);

    // Layout for password input
    auto passwordLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    passwordLayout->addWidget(passwordLaebl);
    passwordLayout->addWidget(passwordEditLine);
    passwordLayout->setSpacing(8);

    auto usernameAndPasswordOverallLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    usernameAndPasswordOverallLayout->insertLayout(0, usernameLayout);
    usernameAndPasswordOverallLayout->insertLayout(1, passwordLayout);
    usernameAndPasswordOverallLayout->setContentsMargins(10, 10, 10, 10);
    usernameAndPasswordOverallLayout->setSpacing(8);

    QPushButton *loginButton = new QPushButton;
    QPushButton *cancelButton = new QPushButton;

    loginButton->setText("Login");
    loginButton->setStyleSheet("background-color: rgb(236, 240, 241);");
    cancelButton->setText("Cancel");
    cancelButton->setStyleSheet("background-color: rgb(236, 240, 241);");

    connect(loginButton, &QPushButton::clicked, this, &MainWindow::verify);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::close);

    auto buttonLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->setSpacing(30);

    auto overalLayout = new QBoxLayout(QBoxLayout::TopToBottom, backgroundLabel);
    overalLayout->setAlignment(Qt::AlignCenter);
    overalLayout->addWidget(titleLabel);
    overalLayout->addSpacing(60);
    overalLayout->insertLayout(1, usernameAndPasswordOverallLayout);
    overalLayout->insertLayout(2, buttonLayout);
    overalLayout->setSpacing(20);


}


void MainWindow::verify()
{

    QString user = usernameEditLine->text();
    QString pass = passwordEditLine->text();

    if (user == QString("admin") && pass == QString("admin"))
    {
        this->hide();
        openMainMenu();
        this->show();
    }
    else
    {
        QMessageBox::warning(this, "Error", "incorrect username or password");
    }
}

void MainWindow::openMainMenu()
{
    MainMenu mainMenu;
    mainMenu.exec();

}


