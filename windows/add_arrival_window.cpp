
#include "add_arrival_window.h"
#include "qlineedit.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "ui_add_arrival_window.h"

AddArrivalWindow::AddArrivalWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddArrivalWindow)
{
    ui->setupUi(this);

    ui->comboBook->lineEdit()->setPlaceholderText("Выберите книгу...");

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Добавить");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setMinimumSize(0, 30);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setMinimumSize(0, 30);
}

AddArrivalWindow::~AddArrivalWindow()
{
    delete ui;
}


void AddArrivalWindow::addArrival()
{
    if (!ui->comboBook->currentText().isEmpty())
    {
        emit addArrivalTriggered(ui->comboBook->currentData(Qt::UserRole).value<std::shared_ptr<jp::Book>>(),
                                 ui->countBook->text().toStdString());
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


void AddArrivalWindow::addItemBook(std::shared_ptr<jp::Book> book)
{
    ui->comboBook->addItem(QString::fromStdString(book->title), QVariant::fromValue(book));
}


void AddArrivalWindow::clear()
{
    ui->comboBook->clear();
}
