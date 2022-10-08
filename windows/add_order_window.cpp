
#include "qlineedit.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "qspinbox.h"
#include "windows/add_order_window.h"
#include "ui_add_order_window.h"

AddOrderWindow::AddOrderWindow(AddClientWindow& acw, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddOrderWindow),
    acw_(acw)
{
    ui->setupUi(this);

    ui->comboBook->lineEdit()->setPlaceholderText("Выберите книгу...");

    ui->tableBookOrder->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableBookOrder->verticalHeader()->hide();
    ui->tableBookOrder->setVisible(false);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Добавить");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setMinimumSize(0, 30);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setMinimumSize(0, 30);
}

AddOrderWindow::~AddOrderWindow()
{
    delete ui;
}


void AddOrderWindow::addTitleBook(std::string title, std::shared_ptr<jp::Book> bookData)
{
    if (bookData->count != 0)
    {
       ui->comboBook->addItem(QString::fromStdString(title), QVariant::fromValue(bookData));
    }
}


void AddOrderWindow::clearBooks()
{
    ui->comboBook->clear();
}


void AddOrderWindow::goToAddClient()
{
    acw_.exec();
}


void AddOrderWindow::addBookInOrder(std::string title)
{
    if (ui->comboBook->currentIndex() != -1)
    {
        std::shared_ptr<jp::Book> book = ui->comboBook->currentData(Qt::UserRole).value<std::shared_ptr<jp::Book>>();

        ui->tableBookOrder->setVisible(true);

        QPushButton* delete_button = new QPushButton("Удалить", this);
        connect(delete_button, SIGNAL(clicked()), this, SLOT(deleteBookFromOrder()));

        QTableWidgetItem* title_ = new QTableWidgetItem;
        title_->setText(QString::fromStdString(title));

        ui->sum->setText(QString::number(ui->sum->text().toInt() + book->price));

        QTableWidgetItem* price = new QTableWidgetItem;
        price->setText(QString::number(book->price));

        QSpinBox* spinCount = new QSpinBox(this);
        spinCount->setMinimum(1);
        spinCount->setMaximum(book->count);
        prevCount_ = 1;
        connect(spinCount, SIGNAL(valueChanged(int)), this, SLOT(countBookChange(int)));

        ui->tableBookOrder->insertRow(0);
        ui->tableBookOrder->setItem(0, 0, title_);
        ui->tableBookOrder->item(0, 0)->setData(Qt::UserRole, QVariant::fromValue(book));
        ui->tableBookOrder->item(0, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->tableBookOrder->setItem(0, 1, price);
        ui->tableBookOrder->item(0, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->tableBookOrder->setCellWidget(0, 2, spinCount);
        ui->tableBookOrder->setCellWidget(0, 3, delete_button);

        int index = ui->comboBook->currentIndex();
        ui->comboBook->setCurrentIndex(-1);
        ui->comboBook->removeItem(index);
    }
}


void AddOrderWindow::countBookChange(int count)
{
    int row = ui->tableBookOrder->currentRow();
    std::shared_ptr<jp::Book> book = ui->tableBookOrder->item(row, 0)->data(Qt::UserRole).value<std::shared_ptr<jp::Book>>();

    if (prevCount_ > count)
    {
        ui->sum->setText(QString::number(ui->sum->text().toInt() - book->price));
    }
    else
    {
        ui->sum->setText(QString::number(ui->sum->text().toInt() + book->price));
    }

    prevCount_ = count;
}


void AddOrderWindow::deleteBookFromOrder()
{
    std::shared_ptr<jp::Book> book = ui->tableBookOrder->item(0, 0)->data(Qt::UserRole).value<std::shared_ptr<jp::Book>>();
    ui->sum->setText(QString::number(ui->sum->text().toInt() - book->price));

    addTitleBook(book->title, book);
    ui->tableBookOrder->removeRow(ui->tableBookOrder->currentRow());

    if (ui->tableBookOrder->rowCount() == 0)
    {
        ui->tableBookOrder->setVisible(false);
    }
}


void AddOrderWindow::accept()
{
    if (!ui->lineNumber->text().isEmpty() && !ui->lineAddress->text().isEmpty() && ui->sum->text() != "0"
        && ui->tableBookOrder->rowCount() > 0)
    {
        std::list<jp::Book> books;
        for (int i{}; i < ui->tableBookOrder->rowCount(); ++i)
        {
            std::shared_ptr<jp::Book> bookFound = ui->tableBookOrder->item(0, 0)->
                                                  data(Qt::UserRole).value<std::shared_ptr<jp::Book>>();
            jp::Book book = *bookFound;
            QSpinBox* count = qobject_cast<QSpinBox*>(ui->tableBookOrder->cellWidget(0, 2));
            book.count = count->text().toInt();
            books.push_back(book);
        }
        emit addOrder(ui->lineNumber->text().toStdString(), ui->lineAddress->text().toStdString(),
                      ui->sum->text().toStdString(), books);
    }
    else
    {
        QMessageBox::critical(this, "Не заполнены поля", "Пожалуйста заполните все поля");
    }
}


void AddOrderWindow::reject()
{
    ui->lineNumber->setText("");
    ui->lineAddress->setText("");
    ui->sum->setText("0");
    ui->comboBook->setCurrentIndex(-1);
    ui->tableBookOrder->clearContents();
    ui->tableBookOrder->setRowCount(0);
    ui->tableBookOrder->setVisible(false);
    AddOrderWindow::hide();
}
