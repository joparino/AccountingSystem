
#ifndef ADD_ARRIVAL_WINDOW_H
#define ADD_ARRIVAL_WINDOW_H

#include <QDialog>

namespace Ui {
class AddArrivalWindow;
}

class AddArrivalWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddArrivalWindow(QWidget *parent = nullptr);
    ~AddArrivalWindow();
    void addItemBook(QString title);

public slots:
    void addArrival();
    void reject();

signals:
    void addArrivalTriggered(QString title, QString count);

private:
    Ui::AddArrivalWindow *ui;
};

#endif
