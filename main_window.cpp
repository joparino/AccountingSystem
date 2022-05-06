#include "main_window.h"
#include "ui_main_window.h"
#include <QLabel>
#include <QTabBar>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
    emit searchTriggered(ui->searchOrder->text());
}


// Добавление строки с данными о заказе в таблицу
void MainWindow::addRowOrder(QString client, QString book, QString adress, QString status, QString date, QString sum)
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

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0, 0, client_);
    ui->tableWidget->setItem(0, 1, book_);
    ui->tableWidget->setItem(0, 2, adress_);
    ui->tableWidget->setItem(0, 3, status_);
    ui->tableWidget->setItem(0, 4, date_);
    ui->tableWidget->setItem(0, 5, sum_);
}


// Добавление строки с данными о книгах в таблицу
void MainWindow::addRowBook(QString book, QString author, QString genre, QString year, QString price, QString count)
{
    QTableWidgetItem* book_ = new QTableWidgetItem;
    book_->setText(book);

    QTableWidgetItem* author_ = new QTableWidgetItem;
    author_->setText(author);

    QTableWidgetItem* genre_ = new QTableWidgetItem;
    genre_->setText(genre);

    QTableWidgetItem* year_ = new QTableWidgetItem;
    year_->setText(year);

    QTableWidgetItem* price_ = new QTableWidgetItem;
    price_->setText(price);

    QTableWidgetItem* count_ = new QTableWidgetItem;
    count_->setText(count);

    ui->tableWidget_2->insertRow(0);
    ui->tableWidget_2->setItem(0, 0, book_);
    ui->tableWidget_2->setItem(0, 1, author_);
    ui->tableWidget_2->setItem(0, 2, genre_);
    ui->tableWidget_2->setItem(0, 3, year_);
    ui->tableWidget_2->setItem(0, 4, price_);
    ui->tableWidget_2->setItem(0, 5, count_);
}


void MainWindow::clearOrder()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}
