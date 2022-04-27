#include "main_window.h"
#include "ui_main_window.h"
#include <QLabel>
#include <QTabBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    lbl2->setText("Заказы поставщики");
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


// Добавление строки с данными о заказе в таблицу
void MainWindow::AddRowOrder(QString name)
{
    QTableWidgetItem* name_ = new QTableWidgetItem;
    name_->setText(name);

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0, 0, name_);
}
