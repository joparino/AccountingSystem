
#ifndef ADD_CLIENT_WINDOW_H
#define ADD_CLIENT_WINDOW_H

#include <QDialog>

namespace Ui {
class AddClientWindow;
}

class AddClientWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddClientWindow(QWidget *parent = nullptr);
    ~AddClientWindow();

public slots:
    void accept();
    void reject();

signals:
    void addClientTriggered(std::string name, std::string number);

private:
    Ui::AddClientWindow *ui;
};

#endif // ADD_CLIENT_WINDOW_H
