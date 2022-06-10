
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "windows/add_arrival_window.h"
#include "windows/add_book_window.h"
#include "windows/add_order_window.h"
#include "windows/order_window.h"
#include "entities/models.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(OrderWindow& ow, AddOrderWindow& aow, AddBookWindow& abw, AddArrivalWindow& aw, QWidget *parent = nullptr);
    ~MainWindow();

    void addRowOrder(std::shared_ptr<jp::Order>& order) noexcept;
    void addRowBook(std::shared_ptr<jp::Book>& book) noexcept;
    void clearOrder() noexcept;
    void clearBook() noexcept;

signals:
    void searchOrderTriggered(QString str);
    void searchBookTriggered(QString str);

public slots:
    void searchOrder();
    void searchBook();
    void goToOrderWindow();
    void goToAddOrderWindow();
    void goToAddBookWindow();
    void goToAddArrivalWindow();

private:
    Ui::MainWindow *ui;
    OrderWindow& ow_;
    AddOrderWindow& aow_;
    AddBookWindow& abw_;
    AddArrivalWindow& aw_;
};
#endif
