
#ifndef ADD_ORDER_WINDOW_H
#define ADD_ORDER_WINDOW_H

#include <QDialog>
#include "qtablewidget.h"
#include "entities/models.h"
#include "windows/add_client_window.h"

namespace Ui {
class AddOrderWindow;
}

class AddOrderWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrderWindow(AddClientWindow& acw, QWidget *parent = nullptr);
    ~AddOrderWindow();
    void addTitleBook(std::string title, std::shared_ptr<jp::Book> bookData);
    void clearBooks();

public slots:
    void goToAddClient();
    void addBookInOrder(QString title);
    void countBookChange(int count);
    void deleteBookFromOrder();
    void accept();
    void reject();

signals:
    void addOrder(QString number, QString address, QString sum, QTableWidget* comboBook);

private:
    Ui::AddOrderWindow *ui;
    AddClientWindow& acw_;
    int prevCount_;
};

#endif
