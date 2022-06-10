
#include "add_arrival_window.h"
#include "qlineedit.h"
#include "qmessagebox.h"
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
    else
    {
        QMessageBox::critical(this, "Не выбрана книга", "Пожалуйста выберите книгу");
    }
}


void AddArrivalWindow::reject()
{
    ui->comboBook->setCurrentIndex(-1);
    ui->countBook->setValue(1);
    AddArrivalWindow::hide();
}


void AddArrivalWindow::addItemBook(QString title)
{
    ui->comboBook->addItem(title);
}
