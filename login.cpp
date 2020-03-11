#include "login.h"
#include "ui_login.h"
#include "sqlpractice.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_2_clicked()
{
    QCoreApplication::exit();
}

void Login::on_pushButton_clicked()
{
    SQLPractice *program = new SQLPractice;
    program->show();
    this->close();
}
