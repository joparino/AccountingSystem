#include "order_window.h"
#include "qpushbutton.h"
#include "ui_order_window.h"

OrderWindow::OrderWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrderWindow)
{
    ui->setupUi(this);

    ui->tableBook->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableBook->verticalHeader()->hide();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Сохранить");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setMinimumSize(0, 30);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setMinimumSize(0, 30);
}

OrderWindow::~OrderWindow()
{
    delete ui;
}


void OrderWindow::addStatus(std::shared_ptr<jp::Status> status)
{
    ui->comboStatus->addItem(QString::fromStdString(status->name));
}


void OrderWindow::clearStatus()
{
    ui->comboStatus->clear();
}


void OrderWindow::add(std::shared_ptr<jp::Order> order)
{
    order_ = order;
    ui->lineNumber->setText(QString::fromStdString(order_->client->phoneNumber));
    ui->lineClient->setText(QString::fromStdString(order_->client->name));
    ui->lineAddress->setText(QString::fromStdString(order_->address));
    ui->comboStatus->setCurrentIndex(order_->status->id - 1);
    ui->sum->setText(QString::number(order->sum));

    for (auto& book : order_->books)
    {
        QTableWidgetItem* title_ = new QTableWidgetItem;
        title_->setText(QString::fromStdString(book.title));

        QTableWidgetItem* author_ = new QTableWidgetItem;
        author_->setText(QString::fromStdString(book.author->name));

        QTableWidgetItem* publisher_ = new QTableWidgetItem;
        publisher_->setText(QString::fromStdString(book.publisher->name));

        QTableWidgetItem* price_ = new QTableWidgetItem;
        price_->setText(QString::number(book.price));

        QTableWidgetItem* count_ = new QTableWidgetItem;
        count_->setText(QString::number(book.count));

        ui->tableBook->insertRow(0);
        ui->tableBook->setItem(0, 0, title_);
        ui->tableBook->item(0, 0)->setData(Qt::UserRole, QVariant::fromValue(book));
        ui->tableBook->item(0, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        ui->tableBook->setItem(0, 1, author_);
        ui->tableBook->item(0, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        ui->tableBook->setItem(0, 2, publisher_);
        ui->tableBook->item(0, 2)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        ui->tableBook->setItem(0, 3, price_);
        ui->tableBook->item(0, 3)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        ui->tableBook->setItem(0, 4, count_);
        ui->tableBook->item(0, 4)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    }

    if (order_->status->id == 1)
    {
        ui->lineAddress->setEnabled(false);
    }
    else if (order_->status->id == 2 || order_->status->id == 5)
    {
        ui->lineNumber->setEnabled(false);
        ui->lineClient->setEnabled(false);
        ui->lineAddress->setEnabled(false);
    }
    else
    {
        ui->lineNumber->setEnabled(true);
        ui->lineClient->setEnabled(true);
        ui->lineAddress->setEnabled(true);
    }
}


void OrderWindow::accept()
{
    std::string address = ui->lineAddress->text().toStdString();
    std::string clientName = ui->lineClient->text().toStdString();
    std::string clientNumber = ui->lineNumber->text().toStdString();
    int status = ui->comboStatus->currentIndex() + 1;
    if (address != order_->address || clientName != order_->client->name ||
        clientNumber != order_->client->phoneNumber || status != order_->status->id)
    {
        order_->address = address;
        order_->client->name = clientName;
        order_->client->phoneNumber = clientNumber;
        emit hasOrderChangedTriggered(order_, status);
    }
}


void OrderWindow::reject()
{
    order_.reset();
    ui->lineNumber->setText("");
    ui->lineClient->setText("");
    ui->lineAddress->setText("");
    ui->sum->setText("0");

    ui->tableBook->clearContents();
    ui->tableBook->setRowCount(0);
    OrderWindow::hide();
}
