
#include <iostream>
#include "qlineedit.h"
#include "windows/add_order_window.h"
#include "ui_add_order_window.h"

AddOrderWindow::AddOrderWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddOrderWindow)
{
    ui->setupUi(this);

    connect(ui->comboBox->lineEdit(), &QLineEdit::textEdited, this, &AddOrderWindow::textBookChanged);
}

AddOrderWindow::~AddOrderWindow()
{
    delete ui;
}


void AddOrderWindow::addTitleBook(QString title)
{
    ui->comboBox->addItem(title);
}


void AddOrderWindow::textBookChanged(QString str1)
{
    //ui->comboBox->showPopup();
    std::string str = str1.toStdString();
    std::cout << str;
}
