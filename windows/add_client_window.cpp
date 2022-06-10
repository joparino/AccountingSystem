#include "add_client_window.h"
#include "qmessagebox.h"
#include "ui_add_client_window.h"

AddClientWindow::AddClientWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddClientWindow)
{
    ui->setupUi(this);
}

AddClientWindow::~AddClientWindow()
{
    delete ui;
}


void AddClientWindow::accept()
{
    if (!ui->lineName->text().isEmpty() && !ui->lineNumber->text().isEmpty())
    {
        emit addClientTriggered(ui->lineName->text().toStdString(), ui->lineNumber->text().toStdString());
    }
    else
    {
        QMessageBox::critical(nullptr, "Ошибка", "Некоторые поля не заполнены");
    }
}


void AddClientWindow::reject()
{
    ui->lineName->setText("");
    ui->lineNumber->setText("");

    AddClientWindow::hide();
}
