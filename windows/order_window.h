
#ifndef ORDER_WINDOW_H
#define ORDER_WINDOW_H

#include <QDialog>
#include "entities/models.h"

namespace Ui {
class OrderWindow;
}

class OrderWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OrderWindow(QWidget *parent = nullptr);
    ~OrderWindow();
    void addStatus(std::shared_ptr<jp::Status> status);
    void clearStatus();
    void add(std::shared_ptr<jp::Order> order);

public slots:
    void accept();
    void reject();

signals:
    void hasOrderChangedTriggered(std::shared_ptr<jp::Order> order, int status);

private:
    Ui::OrderWindow *ui;
    std::shared_ptr<jp::Order> order_;
};

#endif
