
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "entities/models.h"
#include "windows/add_arrival_window.h"
#include "windows/add_book_window.h"
#include "windows/add_order_window.h"
#include "windows/order_window.h"
#include "windows/sales_three_months.h"
#include "windows/top_client.h"
#include "windows/top_selling_books.h"
#include "windows/top_selling_books_month.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(OrderWindow& ow, AddOrderWindow& aow, AddBookWindow& abw,
               AddArrivalWindow& aw, SalesThreeMonths& s, TopSellingBooks& sb,
               TopSellingBooksMonth& sbw, TopClient& tc, QWidget *parent = nullptr);
    ~MainWindow();

    void addRowOrder(const std::shared_ptr<jp::Order>& order) noexcept;
    void addRowBook(const std::shared_ptr<jp::Book>& book) noexcept;
    int setIndex();
    void clearOrder() noexcept;
    void clearBook() noexcept;

signals:
    void logoutTriggered();
    void searchOrderTriggered(std::string str);
    void searchBookTriggered(std::string str);
    void filterTriggered(int status);
    void exportTopSalesTriggered(std::string path);
    void exportTopSellingTriggered(std::string path);
    void exportTopSellingMonthTriggered(std::string path);
    void exportTopClientTriggered(std::string path);

public slots:
    void logout();
    void searchOrder();
    void searchBook();
    void filter();
    void goToOrderWindow();
    void goToAddOrderWindow();
    void goToAddBookWindow();
    void goToBookWindow();
    void goToAddArrivalWindow();
    void goToSales();
    void goToTopSelling();
    void goToTopSellingMonth();
    void goToTopClient();

private:
    Ui::MainWindow *ui;
    OrderWindow& ow_;
    AddOrderWindow& aow_;
    AddBookWindow& abw_;
    AddArrivalWindow& aw_;
    SalesThreeMonths& sales_;
    TopSellingBooks& selling_books_;
    TopSellingBooksMonth& selling_books_month_;
    TopClient& top_client;
};
#endif
