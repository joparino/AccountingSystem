
#include <QLabel>
#include <QTabBar>
#include <QHeaderView>
#include "ui_main_window.h"
#include "main_window.h"


MainWindow::MainWindow(OrderWindow& ow, AddOrderWindow& aow, AddBookWindow& abw,
                       AddArrivalWindow& aw, SalesThreeMonths& s, TopSellingBooks& sb,
                       TopSellingBooksMonth& sbw, TopClient& tc, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      ow_(ow),
      aow_(aow),
      abw_(abw),
      aw_(aw),
      sales_(s),
      selling_books_(sb),
      selling_books_month_(sbw),
      top_client(tc)
{
    ui->setupUi(this);

    ui->tableOrderWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableBookWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableOrderWidget->verticalHeader()->hide();
    ui->tableBookWidget->verticalHeader()->hide();

    QTabWidget *tabw =ui->tabWidget;
    tabw->setTabText(0, "");
    tabw->setTabText(1, "");
    tabw->setTabText(2, "");
    tabw->setTabText(3, "");

    QTabBar *tabbar = tabw->tabBar();

    QLabel *lbl1 = new QLabel();
    lbl1->setText("Заказы клиентов");
    tabbar->setTabButton(0, QTabBar::LeftSide, lbl1);

    QLabel *lbl2 = new QLabel();
    lbl2->setText("Книги");
    tabbar->setTabButton(1, QTabBar::LeftSide, lbl2);

    QLabel *lbl3 = new QLabel();
    lbl3->setText("Отчеты");
    tabbar->setTabButton(2, QTabBar::LeftSide, lbl3);

    QLabel *lbl4 = new QLabel();
    lbl4->setText("Выход");
    tabbar->setTabButton(3, QTabBar::LeftSide, lbl4);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::logout()
{
    this->hide();
    emit logoutTriggered();
}


void MainWindow::searchOrder()
{
    emit searchOrderTriggered(ui->searchOrder->text().toStdString());
}


void MainWindow::searchBook()
{
    emit searchBookTriggered(ui->searchBook->text().toStdString());
}


void MainWindow::filter()
{
    emit filterTriggered(ui->comboStatus->currentIndex() + 1);
}


void MainWindow::goToOrderWindow()
{
    int index = ui->tableOrderWidget->currentRow();
    ow_.add(ui->tableOrderWidget->item(index, 0)->data(Qt::UserRole).value<std::shared_ptr<jp::Order>>());
    ow_.exec();
}


void MainWindow::goToAddOrderWindow()
{
    aow_.exec();
}


void MainWindow::goToAddBookWindow()
{
    abw_.exec();
}


void MainWindow::goToBookWindow()
{
    int index = ui->tableBookWidget->currentRow();
    abw_.setBook(ui->tableBookWidget->item(index, 0)->data(Qt::UserRole).value<std::shared_ptr<jp::Book>>());
    abw_.exec();
}


void MainWindow::goToAddArrivalWindow()
{
    aw_.exec();
}


void MainWindow::goToSales()
{
    sales_.exec();
}


void MainWindow::goToTopSelling()
{
    selling_books_.exec();
}


void MainWindow::goToTopSellingMonth()
{
    selling_books_month_.exec();
}


void MainWindow::goToTopClient()
{
    top_client.exec();
}


void MainWindow::addRowOrder(const std::shared_ptr<jp::Order>& order) noexcept
{

    QTableWidgetItem* client_ = new QTableWidgetItem;
    client_->setText(QString::fromStdString(order->client->name));

    QTableWidgetItem* adress_ = new QTableWidgetItem;
    adress_->setText(QString::fromStdString(order->address));

    QTableWidgetItem* status_ = new QTableWidgetItem;
    status_->setText(QString::fromStdString(order->status->name));

    QTableWidgetItem* date_ = new QTableWidgetItem;
    date_->setText(QString::fromStdString(order->date));

    QTableWidgetItem* sum_ = new QTableWidgetItem;
    sum_->setText(QString::number(order->sum));

    ui->tableOrderWidget->insertRow(ui->tableOrderWidget->rowCount());
    int row = ui->tableOrderWidget->rowCount() - 1;
    ui->tableOrderWidget->setItem(row, 0, client_);
    ui->tableOrderWidget->item(row, 0)->setData(Qt::UserRole, QVariant::fromValue(order));
    ui->tableOrderWidget->item(row, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableOrderWidget->setItem(row, 1, adress_);
    ui->tableOrderWidget->item(row, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableOrderWidget->setItem(row, 2, status_);
    ui->tableOrderWidget->item(row, 2)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableOrderWidget->setItem(row, 3, date_);
    ui->tableOrderWidget->item(row, 3)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableOrderWidget->setItem(row, 4, sum_);
    ui->tableOrderWidget->item(row, 4)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}


void MainWindow::addRowBook(const std::shared_ptr<jp::Book>& book) noexcept
{
    QTableWidgetItem* book_ = new QTableWidgetItem;
    book_->setText(QString::fromStdString(book->title));

    QTableWidgetItem* author_ = new QTableWidgetItem;
    author_->setText(QString::fromStdString(book->author->name));

    QTableWidgetItem* genre_ = new QTableWidgetItem;
    genre_->setText(QString::fromStdString(book->genre->name));

    QTableWidgetItem* publisher_ = new QTableWidgetItem;
    publisher_->setText(QString::fromStdString(book->publisher->name));

    QTableWidgetItem* year_ = new QTableWidgetItem;
    year_->setText(QString::fromStdString(book->year));

    QTableWidgetItem* price_ = new QTableWidgetItem;
    price_->setText(QString::number(book->price));

    QTableWidgetItem* count_ = new QTableWidgetItem;
    count_->setText(QString::number(book->count));

    ui->tableBookWidget->insertRow(ui->tableBookWidget->rowCount());
    int row = ui->tableBookWidget->rowCount() - 1;
    ui->tableBookWidget->setItem(row, 0, book_);
    ui->tableBookWidget->item(row, 0)->setData(Qt::UserRole, QVariant::fromValue(book));
    ui->tableBookWidget->item(row, 0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableBookWidget->setItem(row, 1, author_);
    ui->tableBookWidget->item(row, 1)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableBookWidget->setItem(row, 2, genre_);
    ui->tableBookWidget->item(row, 2)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableBookWidget->setItem(row, 3, publisher_);
    ui->tableBookWidget->item(row, 3)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableBookWidget->setItem(row, 4, year_);
    ui->tableBookWidget->item(row, 4)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableBookWidget->setItem(row, 5, price_);
    ui->tableBookWidget->item(row, 5)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->tableBookWidget->setItem(row, 6, count_);
    ui->tableBookWidget->item(row, 6)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}


int MainWindow::setIndex()
{
    return ui->comboStatus->currentIndex() + 1;
}


void MainWindow::clearOrder() noexcept
{
    ui->tableOrderWidget->clearContents();
    ui->tableOrderWidget->setRowCount(0);
}


void MainWindow::clearBook() noexcept
{
    ui->tableBookWidget->clearContents();
    ui->tableBookWidget->setRowCount(0);
}
