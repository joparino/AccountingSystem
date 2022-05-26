
#ifndef ADD_ORDER_WINDOW_H
#define ADD_ORDER_WINDOW_H

#include <QDialog>

namespace Ui {
class AddOrderWindow;
}

class AddOrderWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrderWindow(QWidget *parent = nullptr);
    ~AddOrderWindow();
    void addTitleBook(QString title);

private:
    Ui::AddOrderWindow *ui;
};

#endif
