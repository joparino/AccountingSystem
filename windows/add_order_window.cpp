
#include "qlineedit.h"
#include "windows/add_order_window.h"
#include "ui_add_order_window.h"

AddOrderWindow::AddOrderWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddOrderWindow)
{
    ui->setupUi(this);
}

AddOrderWindow::~AddOrderWindow()
{
    delete ui;
}


void AddOrderWindow::addTitleBook(QString title)
{
    ui->comboBox->addItem(title);
}
