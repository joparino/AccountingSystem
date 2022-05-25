
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

public slots:
    void textBookChanged(QString str1);

private:
    Ui::AddOrderWindow *ui;
};

#endif
