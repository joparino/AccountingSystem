
#include "add_arrival_window.h"
#include "qlineedit.h"
#include "ui_add_arrival_window.h"

AddArrivalWindow::AddArrivalWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddArrivalWindow)
{
    ui->setupUi(this);

    ui->comboBook->lineEdit()->setPlaceholderText("Выберите книгу...");
}

AddArrivalWindow::~AddArrivalWindow()
{
    delete ui;
}


void AddArrivalWindow::addArrival()
{
    if (!ui->comboBook->currentText().isEmpty())
    {
        emit addArrivalTriggered(ui->comboBook->currentText(), ui->countBook->text());
    }
}


void AddArrivalWindow::addItemBook(QString title)
{
    ui->comboBook->addItem(title);
}
