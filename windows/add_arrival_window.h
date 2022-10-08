
#ifndef ADD_ARRIVAL_WINDOW_H
#define ADD_ARRIVAL_WINDOW_H

#include <QDialog>
#include "entities/models.h"

namespace Ui {
class AddArrivalWindow;
}

class AddArrivalWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddArrivalWindow(QWidget *parent = nullptr);
    ~AddArrivalWindow();
    void addItemBook(std::shared_ptr<jp::Book> book);
    void clear();

public slots:
    void addArrival();
    void reject();

signals:
    void addArrivalTriggered(std::shared_ptr<jp::Book>, std::string count);

private:
    Ui::AddArrivalWindow *ui;
};

#endif
