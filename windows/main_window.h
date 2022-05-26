
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "windows/add_arrival_window.h"
#include "windows/add_book_window.h"
#include "windows/add_order_window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(AddOrderWindow& ow, AddBookWindow& bw, AddArrivalWindow& aw, QWidget *parent = nullptr);
    ~MainWindow();

    void addRowOrder(QString client, QString book, QString adress,
                     QString status, QString date, QString sum) noexcept;
    void addRowBook(QString book, QString author, QString genre,
                    QString publisher, QString year, QString price, QString count) noexcept;
    void clearOrder() noexcept;
    void clearBook() noexcept;

signals:
    void searchOrderTriggered(QString str);
    void searchBookTriggered(QString str);

public slots:
    void searchOrder();
    void searchBook();
    void goToAddOrderWindow();
    void goToAddBookWindow();
    void goToAddArrivalWindow();

private:
    Ui::MainWindow *ui;
    AddOrderWindow& ow_;
    AddBookWindow& bw_;
    AddArrivalWindow& aw_;
};
#endif // MAINWINDOW_H
