
#include <QLabel>
#include <QTabBar>
#include <QHeaderView>
#include "ui_main_window.h"
#include "main_window.h"
#include "add_order_window.h"
#include "add_book_window.h"


MainWindow::MainWindow(AddOrderWindow& ow, AddBookWindow& bw, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      ow_(ow),
      bw_(bw)
{
    ui->setupUi(this);

    ui->tableOrderWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableBookWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableOrderWidget->horizontalHeader()->hide();
    ui->tableOrderWidget->verticalHeader()->hide();

    ui->tableBookWidget->horizontalHeader()->hide();
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


void MainWindow::searchOrder()
{
    emit searchOrderTriggered(ui->searchOrder->text());
}


void MainWindow::searchBook()
{
    emit searchBookTriggered(ui->searchBook->text());
}


void MainWindow::goToAddOrderWindow()
{
    ow_.exec();
}


void MainWindow::goToAddBookWindow()
{
    bw_.exec();
}


// Добавление строки с данными о заказе в таблицу
void MainWindow::addRowOrder(QString client, QString book, QString adress, QString status, QString date, QString sum) noexcept
{
    QTableWidgetItem* client_ = new QTableWidgetItem;
    client_->setText(client);

    QTableWidgetItem* book_ = new QTableWidgetItem;
    book_->setText(book);

    QTableWidgetItem* adress_ = new QTableWidgetItem;
    adress_->setText(adress);

    QTableWidgetItem* status_ = new QTableWidgetItem;
    status_->setText(status);

    QTableWidgetItem* date_ = new QTableWidgetItem;
    date_->setText(date);

    QTableWidgetItem* sum_ = new QTableWidgetItem;
    sum_->setText(sum);

    ui->tableOrderWidget->insertRow(0);
    ui->tableOrderWidget->setItem(0, 0, client_);
    ui->tableOrderWidget->setItem(0, 1, book_);
    ui->tableOrderWidget->setItem(0, 2, adress_);
    ui->tableOrderWidget->setItem(0, 3, status_);
    ui->tableOrderWidget->setItem(0, 4, date_);
    ui->tableOrderWidget->setItem(0, 5, sum_);
}


// Добавление строки с данными о книгах в таблицу
void MainWindow::addRowBook(QString book, QString author, QString genre, QString year, QString publisher, QString price, QString count) noexcept
{
    QTableWidgetItem* book_ = new QTableWidgetItem;
    book_->setText(book);

    QTableWidgetItem* author_ = new QTableWidgetItem;
    author_->setText(author);

    QTableWidgetItem* genre_ = new QTableWidgetItem;
    genre_->setText(genre);

    QTableWidgetItem* publisher_ = new QTableWidgetItem;
    publisher_->setText(publisher);

    QTableWidgetItem* year_ = new QTableWidgetItem;
    year_->setText(year);

    QTableWidgetItem* price_ = new QTableWidgetItem;
    price_->setText(price);

    QTableWidgetItem* count_ = new QTableWidgetItem;
    count_->setText(count);

    ui->tableBookWidget->insertRow(0);
    ui->tableBookWidget->setItem(0, 0, book_);
    ui->tableBookWidget->setItem(0, 1, author_);
    ui->tableBookWidget->setItem(0, 2, publisher_);
    ui->tableBookWidget->setItem(0, 3, genre_);
    ui->tableBookWidget->setItem(0, 4, year_);
    ui->tableBookWidget->setItem(0, 5, price_);
    ui->tableBookWidget->setItem(0, 6, count_);
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
